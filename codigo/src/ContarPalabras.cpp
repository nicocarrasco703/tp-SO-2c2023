#include <iostream>

#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << " <threads_lectura> <threads_maximo>" << std::endl;
        std::cout << "    " << "<archivo1> [<archivo2>...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    threads_lectura: "
            << "Cantidad de threads a usar para leer archivos." << std::endl;
        std::cout << "    threads_maximo: "
            << "Cantidad de threads a usar para computar máximo." << std::endl;
        std::cout << "    archivo1, archivo2...: "
            << "Archivos a procesar." << std::endl;
        return 1;
    }
    int cantThreadsLectura = std::stoi(argv[1]);
    int cantThreadsMaximo = std::stoi(argv[2]);

    std::vector<std::string> filePaths = {};
    for (int i = 3; i < argc; i++) {
        filePaths.push_back(argv[i]);
    }

    HashMapConcurrente hashMap{}; // = HashMapConcurrente();
//    cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);
//    auto maximo = hashMap.maximoParalelo(cantThreadsMaximo);
    //Modificacion usando la libreria chrono para contabilizar el tiempo de ejecucion.
    std::chrono::time_point<std::chrono::system_clock> startArchivos, endArchivos;
    startArchivos = std::chrono::system_clock::now();
    cargarMultiplesArchivos(hashMap, cantThreadsLectura, filePaths);
    endArchivos = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempoArchivos = endArchivos - startArchivos;
    printf( "%lf ", tiempoArchivos.count());

    std::chrono::time_point<std::chrono::system_clock> startMax, endMax;
    startMax = std::chrono::system_clock::now();
    auto maximo = hashMap.maximoParalelo(cantThreadsMaximo);
    endMax = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempoMax = endMax - startMax;
    printf( "%lf ", tiempoMax.count());

    std::cout << maximo.first << " " << maximo.second << std::endl;

    return 0;
}
