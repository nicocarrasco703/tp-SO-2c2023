""" Experimento 2 - Distribucion no equitativa - Maximo Paralelo
    Se busca analizar que sucede cuando no hay una distribucion equitativa en la letra inicial de las
    palabras.

    Para correr el script, primero es necesario crear el ejecutable Experiment.
    Este se crea corriendo "make Experiment" en la carpeta principal.
"""

import subprocess as sp
import numpy as np

# configuracion de plots
from matplotlib import pyplot as plt
import pandas as pd
import ast
from collections import Counter
import string


def runSourceCode(threads_lectura, threads_maximo, cantArchivos, archivos):
    ejecutable = "../build/Experiment"
    comando = [ejecutable, str(threads_lectura), str(threads_maximo)]

    for i in range(0, cantArchivos):
        comando.append("../data/"+ archivos[i])
    print(comando)
    res = sp.check_output(comando, encoding="utf8")
    print("Resultado de c++:")
    print(res)
    tiemposMaximos = ast.literal_eval(res)

    return tiemposMaximos

def scatterPlot(ejeY, ejeX, color, label,totalThreads):
    print("Plotting...")
    plt.figure(figsize=(12,12), dpi= 80)

    plt.scatter(ejeX, ejeY, s=30, alpha=1, color=color)

    #plt.yticks(ejeY)
    plt.xticks(np.arange(0, totalThreads+1, 1.0),fontsize=15)
    plt.yticks(fontsize=15)
    plt.title(label, fontdict={'size':20})
    plt.xlabel('Cantidad de threads', fontsize=20)
    plt.ylabel("Tiempo (s)", fontsize=20)
    plt.grid(linestyle='--')
    plt.legend()
    plt.savefig("../Graficos/grafico_"+label+".png")


def experimentar():

    totalThreads = 8 # Elegir el maximo de cantidad de threads
    archivos = ["wordset-experimento2.txt"] # elegir con que archivo se busca experimentar

    tiempos_max = runSourceCode(5,totalThreads,len(archivos), archivos) # devuelve una lista con los tiempos por cantidad de threads
    df = pd.DataFrame(data={
        'CantThreads': range(1,totalThreads+1),
        'Maximo': tiempos_max
    })
    scatterPlot(df.Maximo, df.CantThreads, "red", "Tiempo de ejecucion de maximoParalelo - Archivo no equitativo",totalThreads)


experimentar()