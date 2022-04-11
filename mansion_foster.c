#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <stdbool.h> 
#include <string.h>
#include"la_sombra_de_mac.h"   
#include"utiles.h"
#include"comandos.h"  

#define CANTIDAD_LEIDOS 5
#define MAX_ELECCION 50
                                                           


int main(int argc, char* argv[]){
    
    srand((unsigned)time(NULL));
   
    char* eleccion;
    char* archivo;

    bool configuracion_activa = false;
    

    if(argc > 1){
        eleccion = argv[1];
        if(argc > 2){
            archivo = argv[2];
        }
        
        
        if(strcmp(eleccion, "agregar_partida") == 0){
            partida_t partida_a_agregar;
            agregar_partida(archivo, partida_a_agregar);
        }
        
        if(strcmp(eleccion, "eliminar_partida") == 0){
            partida_t partida_a_eliminar;
            strcpy(partida_a_eliminar.jugador, argv[3]);
            eliminar_partida(archivo, partida_a_eliminar.jugador);
        }
        
        if(strcmp(eleccion, "ordenar_partidas") == 0){
            procedimiento_ordenar_partidas(archivo);
        }
        
        if(strcmp(eleccion, "config_juego") == 0){
            configuracion_activa = true;
            inicializar_partida(configuracion_activa);
        }

        
    }
    else if(argc == 1){
        inicializar_partida(configuracion_activa);
    }
        
   
  
       
    return 0;
}