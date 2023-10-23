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
    mutexes[bucketIndex].lock();
    incrementarEnLista(tabla[bucketIndex], clave);
    mutexes[bucketIndex].unlock();
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

    for(const auto& element :*lista){
        if(element.first == clave){
            return element.second;
        }
    }
    return 0; //En caso de no encontrar, se devuelve 0.
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
    }

    return *max;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    // Completar (Ejercicio 3)
}

#endif
