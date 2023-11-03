#include <iostream>
#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"

int main(int argc, char **argv) {
    if (argc < 4) {
        return 1;
    }
    int cantThreadsLectura = std::stoi(argv[1]);
    int cantThreadsMaximo = std::stoi(argv[2]);

    std::vector<std::string> filePaths = {};
    for (int i = 3; i < argc; i++) {
        filePaths.push_back(argv[i]);
    }

    HashMapConcurrente hashMap{};

    std::chrono::time_point<std::chrono::system_clock> startArchivos, endArchivos;
    startArchivos = std::chrono::system_clock::now();

    cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);

    endArchivos = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempoArchivos = endArchivos - startArchivos;

    std::vector<double> tiemposMaximo(cantThreadsMaximo);

    for (int i = 0; i < cantThreadsMaximo ; ++i) {
        for (int j = 0; j < 5 ; ++j) {
            std::chrono::time_point<std::chrono::system_clock> startMax, endMax;
            startMax = std::chrono::system_clock::now();

            auto maximo = hashMap.maximoParalelo(i+1);

            endMax = std::chrono::system_clock::now();
            std::chrono::duration<double> tiempoMax = endMax - startMax;

            tiemposMaximo[i]+= tiempoMax.count();
        }
        tiemposMaximo[i] /= 5;
    }
    std::cout << "[ ";
    for (double i: tiemposMaximo)
        std::cout<< i << ", ";
    std::cout << "] "<<std::endl;

    return 0;
}
