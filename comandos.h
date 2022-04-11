#ifndef __COMANDOS_H__
#define __COMANDOS_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include"la_sombra_de_mac.h"
#include"utiles.h"

#define MAX_NOMBRE 100
#define MAX_PARTIDAS 10000

typedef struct partida {
    char jugador[MAX_NOMBRE];
    int nivel_llegado;
    int puntos;
    int vidas_restantes;
    bool gano;
}partida_t;

/*
 * Recibe un archivo que contiene informacion sobre las partidas jugadas
 * Copia la informacion del archivo principal ya que se lo abre en modo lectura a uno auxiliar en modo escritura en el que le agregará la partida nueva
 * Cuando el nombre nuevo sea menor a un nombre leido, se imprimirán en el archivo aux los datos
 * Se imprime la partida leida 
 * Luego se intercambian los nombres de los archivos, para que el archivo original quede con todos los datos
 * Se cierran los archivos
 * Devuelve EXITO si pudo completar la tarea, FALLO en el caso que no pueda.
 */
int agregar_partida(char* archivo, partida_t partida_a_agregar);

/*
 * Recibe el archivo que contiene la informacion de las partidas jugadas y el nombre del jugador correspondiente a la partida a eliminar
 * Abriendo u archivo auxiliar en modo escritura, se copia toda la informacion menos la del jugador a eliminar
 * Se intercambian los nombres de los archivos para que el origuanl quede con los nuevos datos, es decir, sin la partida eliminada
 * Se cierran los archivos
 * Devuelve EXITO si pudo completar la tarea, FALLO en el caso que no pueda.
 */
int eliminar_partida(char* archivo, char jugador[MAX_NOMBRE]);

/*
 * Recibe el archivo para ordenar
 * Lee el archivo original y guarda los valores de cada partida en un vector de partidas
 * Se compara los nombres de los jugadores de cada partida y se reordenan de menor a mayor
 * La lista ya ordenada se escribe en un archivo auxiliar
 * Se renombran los archivos asi el archivo original obtiene el contenido ordenado del archivo auxiliar
 * Se cierran ambos archivos
 * Devuelve EXITO si pudo completar la tarea, FALLO en el caso que no pueda.
 */
int procedimiento_ordenar_partidas(char* archivo);



#endif/*__COMANDOS_H__*/