#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
#include <iostream>
#include <fstream>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    // Completar (Ejercicio 2)
    unsigned int bucketIndex = hashIndex(clave);
    accesoBucket[bucketIndex].lock();
    incrementarEnLista(tabla[bucketIndex], clave);
    accesoBucket[bucketIndex].unlock();
}

void HashMapConcurrente::incrementarEnLista(ListaAtomica<hashMapPair> *lista, std::string clave) {
    for (auto& element: *lista) {
        if(element.first == clave){ // Si la clave ya existe la incrementamos
            element.second++;
            return;
        }
    }
    // Si no esta la clave se agrega
    lista->insertar(std::make_pair(clave, 1));
}

std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
    std::vector<std::string> claves;
    for(const auto& lista : tabla){
        for(const auto& element : *lista){
            claves.push_back(element.first);
        }
    }

    return claves;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
    auto *lista = tabla[hashIndex(clave)]; // Lista de la clave.

    accesoBucket[hashIndex(clave)].lock();
    for(const auto& element :*lista){
        if(element.first == clave){
            auto res = element.second;
            accesoBucket[hashIndex(clave)].unlock();
            return res;
        }
    }
    accesoBucket[hashIndex(clave)].unlock();
    return 0; //En caso de no encontrar, se devuelve 0.
}

//Habria que ver si al pedir que no sea bloqueante, deberiamos buscar el maximo de todas las listas de una vez o tipo ir
// cerrando los mutex cada vez que busco en una lista, y asi ir las desbloqueando. Si es asi deberiamos cambiarlo
// tambien en maximo paralelo. La diferencia estaria en que liberariamos el mutex de la letra, en cada ciclo.
hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;
    // Bloquear todas las entradas
    for(auto& mut: accesoBucket){
        mut.lock();
    }

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
        // Liberar la que ya fue inspeccionada
        accesoBucket[index].unlock();
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
    for(auto& mut: accesoBucket){
        mut.lock();
    }

    std::atomic<int> idxActual(0);
    auto *maximoActual = new hashMapPair();
    maximoActual->second = 0;
    std::mutex mtx_maximo;
    std::vector<std::thread> threads(cant_threads);

    for (auto& t: threads) {
        t = std::thread(&HashMapConcurrente::buscarMaximoThread, this , &idxActual, maximoActual, &mtx_maximo);
    }

    for (auto& t : threads) {
        t.join();
    }

    for(auto& mtx: accesoBucket){
        mtx.unlock();
    }

    return *maximoActual;

}

void HashMapConcurrente::buscarMaximoThread(std::atomic<int> *idxActual, hashMapPair *maximoActual,
                                            std::mutex *mtx_maximo) {

    while(*idxActual < HashMapConcurrente::cantLetras){
        int index = (*idxActual).fetch_add(1);
        if(index >= HashMapConcurrente::cantLetras) {
            break; //En caso de que de alguna forma, al sumar, superen cantletras.
        }
        //Busco el maximo de la lista.
        hashMapPair max = HashMapConcurrente::maximoLista(index);

        //Actualizo el maximo.
        mtx_maximo->lock();
        if(max.second > maximoActual->second){
            maximoActual->first = max.first;
            maximoActual->second = max.second;
        }
        mtx_maximo->unlock();
    }

}
// Metodo para obtener el maximo de una lista.
hashMapPair HashMapConcurrente::maximoLista(int index) {
    auto *max = new hashMapPair();
    max->second = 0;

    for (auto &p : *tabla[index]) {
        if (p.second > max->second) {
            max->first = p.first;
            max->second = p.second;
        }
    }

    return *max;
}

#endif
