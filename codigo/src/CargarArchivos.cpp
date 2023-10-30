#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        // Completar (Ejercicio 4)
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    // Completar (Ejercicio 4)
    std::atomic<int> indiceArchivo(0);
    std::vector<std::thread> threads(cantThreads);

    for (auto& t: threads) {
        t = std::thread([&]{
            while (indiceArchivo < filePaths.size()) {
                int index = (indiceArchivo).fetch_add(1);
                if (index >= filePaths.size()) {
                    break;
                }
                cargarArchivo(hashMap, filePaths[index]);
            }
        });

    }

    for (auto& t : threads) {
        t.join();
    }
}



#endif
