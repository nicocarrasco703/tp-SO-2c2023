"""Experimento 3 - Carga de archivos - Archivos grandes
    Se busca analizar como varia la carga de archivos con distinta cantidad de
    threads.

    Para correr el script, primero es necesario crear el ejecutable ContarPalabras.
    Este se crea corriendo make en la carpeta principal.
"""

import subprocess as sp
import numpy as np
# configuracion de plots
from matplotlib import pyplot as plt
import pandas as pd


def tprun(threads_lectura, threads_maximo, cantArchivos):
    iteraciones = 10
    ejecutable = "../build/ContarPalabras"
    comando = [ejecutable, str(threads_lectura), str(threads_maximo)]
    characters = 'abcdefghijklmnñopqrstuvwxyz'
    for i in range(0, cantArchivos):
        comando.append("../data/experimento3/"+ characters[i] +".txt")
    print(comando)
    promedio_archivos = []
    promedio_max = []
    for i in range(1,iteraciones):
        res = sp.check_output(comando, encoding="utf8")
        [tiempo_archivos, tiempo_max, _, _] = res.split()
        promedio_archivos.append(float(tiempo_archivos))
        promedio_max.append(float(tiempo_max))
    return [np.ma.median(promedio_archivos), np.ma.median(promedio_max)]

def scatterPlot(ejeY, ejeX, color, label):
    print("Plotting...")
    plt.figure(figsize=(12,12), dpi= 80)
    
    plt.scatter(ejeX, ejeY, s=30, alpha=1, color=color)

    plt.xticks(fontsize=15)
    plt.yticks(fontsize=15)
    plt.title(label, fontdict={'size':20})
    plt.xlabel('Cantidad de threads', fontsize=20)

    plt.ylabel("Tiempo (s)", fontsize=20)
    plt.grid(linestyle='--')
 
    # plt.legend()
    plt.savefig("../Graficos/grafico_"+label+".png")
    plt.show()


def experimento_variando_threads():
    tiempos_archivos = []
    tiempos_max = []
    totalThreads = 6
    threadsLectura = 16
    for threads in range(1,threadsLectura):
        print("Corriendo con ", threads, " threads")
        [tiempo_archivos, tiempo_max] = tprun(threads, totalThreads, 12)
        tiempos_archivos.append(float(tiempo_archivos))
        tiempos_max.append(float(tiempo_max))
    df = pd.DataFrame(data={
        'CantThreads': range(1,threadsLectura),
        'Archivos': tiempos_archivos,
        'Maximo': tiempos_max
    })
    scatterPlot(df.Archivos, df.CantThreads, "red", "Tiempo de carga de archivos en paralelo - archivos grandes")

experimento_variando_threads()

    