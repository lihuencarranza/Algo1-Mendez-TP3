#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include <string.h>
#include"la_sombra_de_mac.h"
#include"utiles.h"
#include"comandos.h"

#define MAC 'M' 
#define BLOO 'B' 
#define MAC_Y_BLOO 'X'
#define TERRENO '-' 

#define EMOJI_MAC "\U0001f9d2"
#define EMOJI_BLOO "\U0001f47b"
#define EMOJI_TERRENO "\u2B1B"

#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define VIDA 'V'

#define LLAVE 'L'
#define MONEDA 'C'
#define POZO 'W'
#define INTERRUPTOR 'O'
#define VELA 'V'
#define PUERTA 'D'
#define PORTAL 'P'
#define ESCALERA 'E'
#define PARED 'R'
#define VELITA 'B'

#define EMOJI_LLAVE "\U0001f511"
#define EMOJI_MONEDA "\U0001fa99"
#define EMOJI_POZO "\U0001f573"
#define EMOJI_INTERRUPTOR "\U0001f4cd"
#define EMOJI_VELA "\U0001f56f"
#define EMOJI_PUERTA "\U0001f6aa"
#define EMOJI_PORTAL "\U0001f365"
#define EMOJI_ESCALERA "\U0001fa9c"
#define EMOJI_VIDA "\u2764"
#define EMOJI_PUNTOS "\U0001f31f"
#define EMOJI_PARED "\U0001f9f1"


#define CANT_VELAS_1 5
#define CANT_VELAS_2 10
#define CANT_VELAS_3 12
#define CANT_POZOS_1 15
#define CANT_POZOS_2 20
#define CANT_POZOS_3 30
#define CANT_INTERRUPTORES_1 1
#define CANT_INTERRUPTORES_2 2
#define CANT_INTERRUPTORES_3 2
#define CANT_PORTALES_1 0
#define CANT_PORTALES_2 2
#define CANT_PORTALES_3 4

#define CANTIDAD_ESCALERAS_1 10
#define CANTIDAD_ESCALERAS_2 15
#define CANTIDAD_ESCALERAS_3 15
#define CANTIDAD_PUERTAS 1
#define CANTIDAD_LLAVE_1 0
#define CANTIDAD_LLAVE_2 1
#define CANTIDAD_LLAVE_3 1
#define CANTIDAD_MONEDAS_1 10
#define CANTIDAD_MONEDAS_2 15
#define CANTIDAD_MONEDAS_3 15

#define LECTURA_CONFG "N%i_%[^=]=%i\n"
#define CANT_CONFIG 3

/*
 *    Función que recibe dos coordenadas, devuelve true si las
 * coordenadas son iguales, sino false.
 */
int distancia_manhatann(coordenada_t a, coordenada_t b){
    int distancia;
    distancia = abs(a.fila - b.fila) + abs(a.col - b.col);
    return distancia;
}

/*
 *    Función que recibe dos coordenadas, devuelve true si las
 * coordenadas son iguales, sino false.
 */
bool coordenadas_iguales(coordenada_t a, coordenada_t b){
    return (a.fila == b.fila && a.col == b.col);
}

/*
 *   Función que dados una altura máxima y un ancho máximo 
 * devuelve una coordenada aleatoria. Una posición nunca podrá
 * tener un valor negativo, ni ser mayor a sus máximos.
 */
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){

    coordenada_t posicion;
    posicion.fila = rand() % max_alto;
    posicion.col = rand() % max_ancho;
    return (posicion);

}

/*
 * Recibe el vector de paredes junto a su tope y una coordenada
 * busca dentro del vector si alguna de las coordenadas de las paredes coincide con las del elemento recibido
 * devuele true si coicide la coordenada recibida con la de una de sus paredes
 */
bool hay_pared(coordenada_t paredes[MAX_PAREDES], int tope_paredes, coordenada_t elemento){
    bool hay = false;
    for(int i = 0; i < tope_paredes; i++){
        if(coordenadas_iguales(paredes[i], elemento)){
            hay = true;
        }
    }
    return hay;
}

/*
 * Recibe el struct inicializado de configuracion
 * se le asignan a las variables las constantes definidas por el juego
 */ 
void configuracion_default(configuracion_t* configuracion){
    configuracion->n1_pozos = CANT_POZOS_1;
    configuracion->n2_pozos = CANT_POZOS_2;
    configuracion->n3_pozos = CANT_POZOS_3;

    configuracion->n1_velas = CANT_VELAS_1;
    configuracion->n2_velas = CANT_VELAS_2;
    configuracion->n3_velas = CANT_VELAS_3;

    configuracion->n1_interruptores = CANT_INTERRUPTORES_1;
    configuracion->n2_interruptores = CANT_INTERRUPTORES_2;
    configuracion->n3_interruptores = CANT_INTERRUPTORES_3;

    configuracion->n1_portales = CANT_PORTALES_1;
    configuracion->n2_portales = CANT_PORTALES_2;
    configuracion->n3_portales = CANT_PORTALES_3;

    configuracion->n1_monedas = CANTIDAD_MONEDAS_1;
    configuracion->n2_monedas = CANTIDAD_MONEDAS_2;
    configuracion->n3_monedas = CANTIDAD_MONEDAS_3;

    configuracion->n1_escaleras = CANTIDAD_ESCALERAS_1;
    configuracion->n2_escaleras = CANTIDAD_ESCALERAS_2;
    configuracion->n3_escaleras = CANTIDAD_ESCALERAS_3;

    configuracion->n1_llaves = CANTIDAD_LLAVE_1;
    configuracion->n2_llaves = CANTIDAD_LLAVE_2;
    configuracion->n3_llaves = CANTIDAD_LLAVE_3;
}

/*
 * Recibe el struct inicializado de configuracion
 * Abre y chequea que exista config.txt
 * Si no exite config.txt se termina el procedimiento
 * Si existe config.txt, lee el archivo y, dependiendo el tipo de elemento se le asignará al struct de la coniguracion un nuevo valor, pisando los predeterminados
 * Lo bueno de esta funcion es que si hay valores que el archivo config.txt no tiene, no pasa nada porque ya teniamos los valor por default asignados
 */
void inicializar_configuracion(configuracion_t* configuracion){
    printf("se incializó config\n");
    FILE* config = fopen("config.txt", "r");
    if(!config){
        perror("No se pudo abrir el archivo original");
    }
    

    int nivel;
    char elemento[MAX_NOMBRE];
    int cantidad;
    int leidos = fscanf(config, LECTURA_CONFG, &nivel, elemento, &cantidad);
    while(leidos == CANT_CONFIG){
        if(strcmp(elemento, "POZOS") == 0){
            if(nivel == 1){
                configuracion->n1_pozos = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_pozos = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_pozos = cantidad;
            }
        }       
        else if(strcmp(elemento, "VELAS") == 0){
            if(nivel == 1){
                configuracion->n1_velas = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_velas = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_velas = cantidad;
            }
        }
        else if(strcmp(elemento, "IN TERRUPTORES") == 0){
            if(nivel == 1){
                configuracion->n1_interruptores = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_interruptores = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_interruptores = cantidad;
            }
        }       
        else if(strcmp(elemento, "PORTALES") == 0){
            if(nivel == 1){
                configuracion->n1_portales = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_portales = cantidad;
            }
            if(nivel == 3){
                configuracion->n3_portales = cantidad;
            }
        }
        else if(strcmp(elemento,"MONEDAS") == 0){
            if(nivel == 1){
                configuracion->n1_monedas = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_monedas = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_monedas = cantidad;
            }
        }
        else if(strcmp(elemento, "ESCALERAS") == 0){
            if(nivel == 1){
                configuracion->n1_escaleras = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_escaleras = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_escaleras = cantidad;
            }            
        }
        else if(strcmp(elemento, "LLAVES") == 0){
            if(nivel == 1){
                configuracion->n1_llaves = cantidad;
            }
            if(nivel == 2){
                configuracion->n2_llaves = cantidad;
            }
            if(nivel == 3){
                configuracion->n2_llaves = cantidad;
            }
        }
        
        leidos = fscanf(config, LECTURA_CONFG, &nivel, elemento, &cantidad);
        printf("se pudió\n"); //yo

    }
    fclose(config);
    
}

/*
 *   La función recibe un caracter y devuelve true
 * si el movimiento recibido concuerda con la convención
 * propuesta, sino false.
 * -> W: Si el personaje debe moverse para la arriba. 
 * -> A: Si el personaje debe moverse para la izquierda.
 * -> S: Si el personaje debe moverse para la abajo.
 * -> D: Si el personaje debe moverse para la derecha.
 * -> V: Para comprar una vida.
 */
bool es_movimiento_valido(char movimiento){
    return (movimiento == ARRIBA || movimiento == ABAJO || movimiento == DERECHA || movimiento == IZQUIERDA || movimiento == VIDA);
}

/*
 *   Procedimineto que se encargará de pedirle al usuario
 * que ingrese un movimiento hasta que sea válido.
 */
void pedir_movimiento(char* ref_movimiento){
    printf("Ingrese movimiento:\n");
    scanf(" %c", ref_movimiento);
}

/*
 *    La función recibe una coordenada, devuelve true
 * si la posición dentro de las dimensiones dadas, sino false.
 * Una posición nunca podrá tener un valor negativo y no tendrá un 
 * valor más alto que los máximos.
 */
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){
    
    return (posicion.fila <= max_alto && posicion.col <= max_ancho && posicion.fila >= 0 && posicion.col >= 0);
}

/*
 *    La función recibe un vector de elementos, su respectivo tope y una coordenada,
 * devuelve la posición del elemento del vector, que coincida con la coordenada pasada 'posicion',
 * si no se encuentra ningún elemento, se devolverá -1. 
 */
int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){
    int posicion_vector = -1;
    for(int i = 0; i < tope; i++){
        if(coordenadas_iguales(posicion, elementos[i].coordenada) == true){
            posicion_vector = i;    
        }
    }
    return posicion_vector;    
}

/*
 * Recibe un vector de elementos junto a su tope y una coordenada
 * si la herramienta fue usada, se elimina del vector
 */
void eliminar_herramienta(elemento_t elementos[MAX_ELEMENTOS], int* tope_elementos, coordenada_t posicion){
    int pos_buscado = buscar_elemento(elementos, (*tope_elementos), posicion);
    if(pos_buscado == -1){
        return;
    }
    for(int i = pos_buscado; i < *tope_elementos -1 ;i++){
        elementos[i] = elementos[i+1];
    }
    (*tope_elementos)--;
    
}

/* 
 * Recibe la información del juego
 * reinicia las posiciones de mac y de la sombra respecto de mac
 */
void reiniciar_posicioner_personajes(juego_t* juego){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_obstaculos; i++){
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == POZO){            juego->personaje.posicion = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
            juego->sombra.posicion.fila = juego->personaje.posicion.fila;
            juego->sombra.posicion.col = MAX_COLUMNAS - 1 - juego->personaje.posicion.col;
        }
    }
}

/*
 *    El procedimiento recibe el personaje y la sombra por referencia, estado del interruptor , el movimiento,
 * y según este último, los mueve acordemente.
 * 
 * El personaje se mueve de la forma:
 * -> W: Si el personaje debe moverse para la arriba. 
 * -> A: Si el personaje debe moverse para la izquierda.
 * -> S: Si el personaje debe moverse para la abajo.
 * -> D: Si el personaje debe moverse para la derecha.
 * -> V: comprar una vida.
 *
 *   La sombra se mueve de igual forma para el eje vertical, pero
 * para el eje horizontal se moverá de forma inversa:
 *
 * -> A: Si el personaje debe moverse para la derecha.
 * -> D: Si el personaje debe moverse para la izquierda.
 *
 * Si el interruptor está encendido la sombra se mueve igual wue el personaje
 */
void mover_personaje(personaje_t* personaje, sombra_t* sombra, char movimiento, bool interruptor_apretado){
   if(interruptor_apretado == false && sombra->esta_viva == true){
        switch(movimiento){
            case IZQUIERDA:
                if(personaje->posicion.col != 0){
                    personaje->posicion.col -= 1;
                }
                if(sombra->posicion.col != MAX_COLUMNAS - 1){
                    sombra->posicion.col += 1;
                }
                break;

            case DERECHA:
                if(personaje->posicion.col != MAX_COLUMNAS - 1){
                (personaje->posicion.col) += 1;
                }
                if(sombra->posicion.col != 0){
                    (sombra->posicion.col) -= 1; 
                }
                break;

            case ABAJO:
                if(personaje->posicion.fila != MAX_FILAS - 1){
                    (personaje->posicion.fila) += 1;
                }
                if(sombra->posicion.fila != MAX_FILAS - 1){
                    (sombra->posicion.fila) += 1;
                }            
                break;

            case ARRIBA:
                if(personaje->posicion.fila != 0){
                    (personaje->posicion.fila) -= 1;
                }
                if(sombra->posicion.fila != 0){
                    (sombra->posicion.fila) -= 1;
                }  
                break;        
            case VIDA:            
                if(personaje->vida < 3 && personaje->puntos >= 200){
                    personaje->puntos -= 200;
                    personaje->vida++; 
                }
                break;
        }
    }
    else if(interruptor_apretado==true && sombra->esta_viva == true){
        switch(movimiento){
            case IZQUIERDA:
                if(personaje->posicion.col != 0){
                    personaje->posicion.col -= 1;
                }
                if(sombra->posicion.col != 0){
                    sombra->posicion.col -= 1;
                }
                break;

            case DERECHA:
                if(personaje->posicion.col != MAX_COLUMNAS - 1){
                (personaje->posicion.col) += 1;
                }
                if(sombra->posicion.col != MAX_COLUMNAS - 1){
                    (sombra->posicion.col) += 1; 
                }
                break;

            case ABAJO:
                if(personaje->posicion.fila != MAX_FILAS - 1){
                    (personaje->posicion.fila) += 1;
                }
                if(sombra->posicion.fila != MAX_FILAS - 1){
                    (sombra->posicion.fila) += 1;
                }            
                break;

            case ARRIBA:
                if(personaje->posicion.fila != 0){
                    (personaje->posicion.fila) -= 1;
                }
                if(sombra->posicion.fila != 0){
                    (sombra->posicion.fila) -= 1;
                }  
                break;        
            case VIDA:            
                if(personaje->vida < 3 && personaje->puntos >= 200){
                    personaje->puntos -= 200;
                    personaje->vida++; 
                }
                break;
        }
    }
    else if(sombra->esta_viva == false){
        switch(movimiento){
            case IZQUIERDA:
                if(personaje->posicion.col != 0){
                    personaje->posicion.col -= 1;
                }
                break;

            case DERECHA:
                if(personaje->posicion.col != MAX_COLUMNAS - 1){
                (personaje->posicion.col) += 1;
                }
                break;

            case ABAJO:
                if(personaje->posicion.fila != MAX_FILAS - 1){
                    (personaje->posicion.fila) += 1;
                }           
                break;

            case ARRIBA:
                if(personaje->posicion.fila != 0){
                    (personaje->posicion.fila) -= 1;
                } 
                break;        
            case VIDA:            
                if(personaje->vida < 3 && personaje->puntos >= 200){
                    personaje->puntos -= 200;
                    personaje->vida++; 
                }
                break;
        }
    }
  
}

/*
 * Recibe la informacion del juego
 * imprime la informacion necesaria para el usuario
 */
void informacion_jugador(juego_t juego){
    printf("%s disponibles: %i\n", EMOJI_VIDA, juego.personaje.vida);
    if(juego.personaje.tiene_llave == true){
        printf("%s llave disponible: SI\n", EMOJI_LLAVE);
    }
    else if(juego.personaje.tiene_llave == false){
        printf("%s llave disponible: NO\n", EMOJI_LLAVE);
    }
    printf("%s puntos: %i\n", EMOJI_PUNTOS, juego.personaje.puntos);
    if(juego.personaje.interruptor_apretado == false){
        printf("%s apretado: NO\n", EMOJI_INTERRUPTOR);
    }
    else if(juego.personaje.interruptor_apretado == true){
        printf("%s apretado: SI\n", EMOJI_INTERRUPTOR);
    }    
    if(juego.sombra.esta_viva == false){
        printf("%s no está viva\n", EMOJI_BLOO);
    }
    else if(juego.sombra.esta_viva == true){
        printf("%s está viva\n", EMOJI_BLOO);
    }
}

/*
 * Procedimiento que recibe el juego e imprime 
  * toda su información por pantalla.
 */
void imprimir_terreno(juego_t juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS];

    system("clear");
    //datos de jugador

    informacion_jugador(juego);   
    
    //matriz
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno[i][j] = TERRENO;
        }
    }
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;
            terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLOO;
                      
            for(int k = 0; k < juego.niveles[juego.nivel_actual-1].tope_obstaculos; k++){                
                if(juego.niveles[juego.nivel_actual-1].obstaculos[k].tipo == POZO){
                    terreno[juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.col] = POZO;
                }
                else if(juego.niveles[juego.nivel_actual-1].obstaculos[k].tipo == VELA){
                terreno[juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.col] = VELA;
                }
                else if(juego.niveles[juego.nivel_actual-1].obstaculos[k].tipo == INTERRUPTOR){ 
                   terreno[juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.col] = INTERRUPTOR;
                }
                else if(juego.niveles[juego.nivel_actual-1].obstaculos[k].tipo == PORTAL){
                    terreno[juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[k].coordenada.col] = PORTAL;
                }
            }
            for(int k = 0; k < juego.niveles[juego.nivel_actual-1].tope_paredes; k++){    
                terreno[juego.niveles[juego.nivel_actual-1].paredes[k].fila][juego.niveles[juego.nivel_actual-1].paredes[k].col] = PARED;
            } 
            for(int k = 0; k < juego.niveles[juego.nivel_actual-1].tope_herramientas; k++){
                if(juego.niveles[juego.nivel_actual-1].herramientas[k].tipo == ESCALERA){
                terreno[juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.col] = ESCALERA;
                }
                else if(juego.niveles[juego.nivel_actual-1].herramientas[k].tipo == PUERTA){
                    terreno[juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.col] = PUERTA;
                }
                else if(juego.niveles[juego.nivel_actual-1].herramientas[k].tipo == MONEDA){
                    terreno[juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.col] = MONEDA;
                }
                else if(juego.niveles[juego.nivel_actual-1].herramientas[k].tipo == LLAVE){
                    terreno[juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[k].coordenada.col] = LLAVE;
                }
                terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;
                terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLOO;
            }
             

            if(terreno[i][j] == PARED){
                printf("%s", EMOJI_PARED);
            }
            if(terreno[i][j] == TERRENO){
                printf("%s", EMOJI_TERRENO);
            }
            if(terreno[i][j] == LLAVE){
                printf("%s", EMOJI_LLAVE);
            }
            if(terreno[i][j] == MONEDA){
                printf("%s ", EMOJI_MONEDA);
            }            
            if(terreno[i][j] == POZO){
                printf("%s ", EMOJI_POZO);
            } 
            if(terreno[i][j] == INTERRUPTOR){
                printf("%s", EMOJI_INTERRUPTOR);
            }                      
            if(terreno[i][j] == VELA){
                printf("%s ", EMOJI_VELA);
            }            
            if(terreno[i][j] == PORTAL){
                printf("%s", EMOJI_PORTAL);
            }
            
            if(terreno[i][j] == ESCALERA){
                printf("%s ", EMOJI_ESCALERA);
            }    
            if(terreno[i][j] == PUERTA){
                printf("%s", EMOJI_PUERTA);
            }
            if(terreno[i][j] == MAC){
                printf("%s", EMOJI_MAC);
            }
            if(terreno[i][j] == BLOO){
                printf("%s", EMOJI_BLOO);
            }
        }
    printf("\n");
    }
}

/*
 * Recibe una coordenada de un elemento, el vector de herramientas y su tope, el vector de obstaculos y su tope
 * en el caso de coincidir la coordenada de un obstaculo o herramienta con la coordenada recibida, devuelve true
 */
bool coordenada_libre(coordenada_t elemento, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas, elemento_t obstaculos[MAX_PAREDES], int tope_obstaculos){

    bool disponibilidad = true;
    //posiciones personajes
    if(buscar_elemento(herramientas, tope_herramientas, elemento) != -1 || 
    buscar_elemento(obstaculos, tope_obstaculos, elemento) != -1){

        disponibilidad = false;
    }
    
    return disponibilidad;
}

/*
 * Recibe el vector de herramientas junto a su tope por referencia, el vector de obstaculos junto a su tope, el vector de paredes junto a su tope, la cantidad del herramientas y el tipo de herramienta a generar
 * En caso de no ser de tipo ESCALERA, se le asigna una coordenada aleatoria que no coincida con una pared, obstaculo o herramienta
 * En caso de ser ESCALERA, la coordenada debe coincidir con una pared y no debe estar sobre otra escalera
 * Una vez generada, aumenta el tope de herramientas
 */
void generar_herramienta(elemento_t herramientas[MAX_ELEMENTOS], int* tope_herramientas, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, coordenada_t paredes[MAX_PAREDES], int tope_paredes, int cantidad, char tipo){
    for(int i = 0; i < cantidad; i++){
        herramientas[(*tope_herramientas)].tipo = tipo;
        
        if(tipo != ESCALERA){
            do{
                herramientas[(*tope_herramientas)].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
            }while(coordenada_libre(herramientas[(*tope_herramientas)].coordenada, obstaculos, tope_obstaculos, herramientas, *tope_herramientas) == false || hay_pared(paredes, tope_paredes, herramientas[(*tope_herramientas)].coordenada) == true);
        }
        if(tipo == ESCALERA){
            do{
                herramientas[(*tope_herramientas)].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
            }while(hay_pared(paredes, tope_paredes, herramientas[(*tope_herramientas)].coordenada) == false || coordenada_libre(herramientas[(*tope_herramientas)].coordenada, obstaculos, tope_obstaculos, herramientas, *tope_herramientas) == false );
        }
        (*tope_herramientas)+=1;
    }
    
}

/* 
 * Recibe el vector de obstaculos junto a su tope por referencia, el vector de herramientas junto a su tope, el vector de paredes junto a su tope, la cantidad del obstaculos y el tipo de herramienta a generar
 * Se le asigna una coordenada aleatoria que no coincida con una pared, obstaculo o herramienta
 * Una vez generado, aumenta el tope de obstaculos
 */
void generar_obstaculo(elemento_t obstaculos[MAX_ELEMENTOS], int* tope_obstaculos, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas, coordenada_t paredes[MAX_PAREDES], int tope_paredes, int cantidad, char tipo){
    for(int i = 0; i < cantidad; i++){
        obstaculos[(*tope_obstaculos)].tipo = tipo;
        
        do{
            obstaculos[(*tope_obstaculos)].coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
        }while(coordenada_libre(obstaculos[(*tope_obstaculos)].coordenada, herramientas, tope_herramientas, obstaculos, *tope_obstaculos) == false ||
        hay_pared(paredes, tope_paredes, obstaculos[(*tope_obstaculos)].coordenada) == true || buscar_elemento(obstaculos, *tope_obstaculos, obstaculos[*tope_obstaculos].coordenada) != -1) ;
     
        (*tope_obstaculos)+=1;
    }
}


/*
 * Recibe el juego por referencia para ser modificado
 *  Declara los topes de las herramientas de cada nivel en cero
 * Genera las herramientas de acuerdo a nivel, cantidad y tipo de herramienta
 */
void obtener_herramientas(juego_t* juego, configuracion_t configuracion){
    
    juego->niveles[0].tope_herramientas = 0;
    juego->niveles[1].tope_herramientas = 0;
    juego->niveles[2].tope_herramientas = 0;

    //NIVEL UNO
    generar_herramienta(juego->niveles[0].herramientas, &juego->niveles[0].tope_herramientas, juego->niveles[0].obstaculos, juego->niveles[0].tope_obstaculos, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_escaleras, ESCALERA);
    generar_herramienta(juego->niveles[0].herramientas, &juego->niveles[0].tope_herramientas, juego->niveles[0].obstaculos, juego->niveles[0].tope_obstaculos, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, CANTIDAD_PUERTAS, PUERTA);
    generar_herramienta(juego->niveles[0].herramientas, &juego->niveles[0].tope_herramientas, juego->niveles[0].obstaculos, juego->niveles[0].tope_obstaculos, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_monedas, MONEDA);
    generar_herramienta(juego->niveles[0].herramientas, &juego->niveles[0].tope_herramientas, juego->niveles[0].obstaculos, juego->niveles[0].tope_obstaculos, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_llaves, LLAVE);

    //NIVEL DOS
    generar_herramienta(juego->niveles[1].herramientas, &juego->niveles[1].tope_herramientas, juego->niveles[1].obstaculos, juego->niveles[1].tope_obstaculos, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_escaleras, ESCALERA);
    generar_herramienta(juego->niveles[1].herramientas, &juego->niveles[1].tope_herramientas, juego->niveles[1].obstaculos, juego->niveles[1].tope_obstaculos, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, CANTIDAD_PUERTAS, PUERTA);
    generar_herramienta(juego->niveles[1].herramientas, &juego->niveles[1].tope_herramientas, juego->niveles[1].obstaculos, juego->niveles[1].tope_obstaculos, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_llaves, LLAVE);
    generar_herramienta(juego->niveles[1].herramientas, &juego->niveles[1].tope_herramientas, juego->niveles[1].obstaculos, juego->niveles[1].tope_obstaculos, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_monedas, MONEDA);

    //NIVEL TRES
    generar_herramienta(juego->niveles[2].herramientas, &juego->niveles[2].tope_herramientas, juego->niveles[2].obstaculos, juego->niveles[2].tope_obstaculos, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_escaleras, ESCALERA);
    generar_herramienta(juego->niveles[2].herramientas, &juego->niveles[2].tope_herramientas, juego->niveles[2].obstaculos, juego->niveles[2].tope_obstaculos, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, CANTIDAD_PUERTAS, PUERTA);
    generar_herramienta(juego->niveles[2].herramientas, &juego->niveles[2].tope_herramientas, juego->niveles[2].obstaculos, juego->niveles[2].tope_obstaculos, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_llaves, LLAVE);
    generar_herramienta(juego->niveles[2].herramientas, &juego->niveles[2].tope_herramientas, juego->niveles[2].obstaculos, juego->niveles[2].tope_obstaculos, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_monedas, MONEDA);
} 

/*
 * Recibe el juego por refencia para ser modificado
 * Declara los topes de los obstaculos de cada nivel en cero
 * Genera los obstaculos de acuerdo a nivel, cantidad y tipo de obstaculo
 */
void obtener_obstaculos(juego_t* juego, configuracion_t configuracion){

    juego->niveles[0].tope_obstaculos = 0;
    juego->niveles[1].tope_obstaculos = 0;
    juego->niveles[2].tope_obstaculos = 0;
   
    //NIVEL UNO
    generar_obstaculo(juego->niveles[0].obstaculos, &juego->niveles[0].tope_obstaculos, juego->niveles[0].herramientas, juego->niveles[0].tope_herramientas, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_interruptores, INTERRUPTOR);
    generar_obstaculo(juego->niveles[0].obstaculos, &juego->niveles[0].tope_obstaculos, juego->niveles[0].herramientas, juego->niveles[0].tope_herramientas, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_pozos, POZO);
    generar_obstaculo(juego->niveles[0].obstaculos, &juego->niveles[0].tope_obstaculos, juego->niveles[0].herramientas, juego->niveles[0].tope_herramientas, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_velas, VELA); 
    generar_obstaculo(juego->niveles[1].obstaculos, &juego->niveles[0].tope_obstaculos, juego->niveles[0].herramientas, juego->niveles[0].tope_herramientas, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, configuracion.n1_portales, PORTAL);

    //NIVEL DOS
    generar_obstaculo(juego->niveles[1].obstaculos, &juego->niveles[1].tope_obstaculos, juego->niveles[1].herramientas, juego->niveles[1].tope_herramientas, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_interruptores, INTERRUPTOR);
    generar_obstaculo(juego->niveles[1].obstaculos, &juego->niveles[1].tope_obstaculos, juego->niveles[1].herramientas, juego->niveles[1].tope_herramientas, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_pozos, POZO);
    generar_obstaculo(juego->niveles[1].obstaculos, &juego->niveles[1].tope_obstaculos, juego->niveles[1].herramientas, juego->niveles[1].tope_herramientas, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_velas, VELA);
    generar_obstaculo(juego->niveles[1].obstaculos, &juego->niveles[1].tope_obstaculos, juego->niveles[1].herramientas, juego->niveles[1].tope_herramientas, juego->niveles[1].paredes, juego->niveles[1].tope_paredes, configuracion.n2_portales, PORTAL);

    //NIVEL TRES
    generar_obstaculo(juego->niveles[2].obstaculos, &juego->niveles[2].tope_obstaculos, juego->niveles[2].herramientas, juego->niveles[2].tope_herramientas, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_interruptores, INTERRUPTOR);
    generar_obstaculo(juego->niveles[2].obstaculos, &juego->niveles[2].tope_obstaculos, juego->niveles[2].herramientas, juego->niveles[2].tope_herramientas, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_pozos, POZO);
    generar_obstaculo(juego->niveles[2].obstaculos, &juego->niveles[2].tope_obstaculos, juego->niveles[2].herramientas, juego->niveles[2].tope_herramientas, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_velas, VELA);
    generar_obstaculo(juego->niveles[2].obstaculos, &juego->niveles[2].tope_obstaculos, juego->niveles[2].herramientas, juego->niveles[2].tope_herramientas, juego->niveles[2].paredes, juego->niveles[2].tope_paredes, configuracion.n3_portales, PORTAL);

}


/*
 * Procedimiento que dejará la estructura personaje_t en un
 * estado válido para iniciar el juego.
 * Tambien chequea que la sombra obtenga una coordenada disponible
 */
void inicializar_personaje(personaje_t* personaje, coordenada_t paredes[MAX_PAREDES], int tope_paredes, sombra_t* sombra, elemento_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas){ 
    do{
        personaje->posicion = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
        sombra->posicion.fila = personaje->posicion.fila;
        sombra->posicion.col = MAX_COLUMNAS - 1 - personaje->posicion.col;
    }while(coordenadas_iguales(sombra->posicion, personaje->posicion));
    personaje->puntos = 0;
    personaje->vida = 3;
    personaje->tiene_llave = false;
    personaje->interruptor_apretado = false;
    
}

/*
 * Procedimiento que dejará una estructura elemento_t que
 * represente a la sombra, en un estado válido para iniciar el juego. 
 */
void inicializar_sombra(sombra_t* sombra){
    sombra->esta_viva = true;
}


void inicializar_juego(juego_t* juego, configuracion_t configuracion){

    juego->nivel_actual = 1;
    
    cargar_mapas();
    obtener_mapa(juego->niveles[0].paredes, &juego->niveles[0].tope_paredes);
    obtener_mapa(juego->niveles[1].paredes, &juego->niveles[1].tope_paredes);
    obtener_mapa(juego->niveles[2].paredes, &juego->niveles[2].tope_paredes);
    
    obtener_herramientas(juego, configuracion);
    obtener_obstaculos(juego, configuracion);

    
    inicializar_personaje(&juego->personaje, juego->niveles[0].paredes, juego->niveles[0].tope_paredes, &juego->sombra, juego->niveles[0].obstaculos, juego->niveles[0].tope_obstaculos, juego->niveles[0].herramientas, juego->niveles[0].tope_herramientas);
    inicializar_sombra(&juego->sombra);


    
}


int estado_juego(juego_t juego){ 
    int estado = 0;

    if(juego.personaje.vida == 0){
        estado = -1;
        system("clear");
        printf(" \
  ____    _____   ____    ____    ___   ____    _____   _____      \n \
 |  _ \\  | ____| |  _ \\  |  _ \\  |_ _| / ___|  |_   _| | ____|  \n \
 | |_) | |  _|   | |_) | | | | |  | |  \\___ \\    | |   |  _|     \n \
 |  __/  | |___  |  _ <  | |_| |  | |   ___) |   | |   | |___      \n \
 |_|     |_____| |_| \\_\\ |____/  |___| |____/    |_|   |_____|\n");
    }
    
    if(juego.nivel_actual == 3 && estado_nivel(juego) == 1 && juego.personaje.vida > 0 && juego.sombra.esta_viva == true){
        estado = 1;
        printf(" \
   ____      _      _   _      _      ____    _____   _____        \n \
  / ___|    / \\    | \\ | |    / \\    / ___|  |_   _| | ____|    \n \
 | |  _    / _ \\   |  \\| |   / _ \\   \\___ \\    | |   |  _|    \n \
 | |_| |  / ___ \\  | |\\  |  / ___ \\   ___) |   | |   | |___     \n \
  \\____| /_/   \\_\\ |_| \\_| /_/   \\_\\ |____/    |_|   |_____| \n");
    }
    
    return estado;
}

/* 
 * Recibe el vector de herramientas y su tope
 * Busca la posision del elemento PUERTA dentro del vector de herramientas
 * Devuelve la posicion de la puerta dentro del vector de herramientas
 */
int buscar_puerta(elemento_t herramientas[MAX_ELEMENTOS], int tope_herramientas){
    int posicion_vector = -1;
    for(int i = 0; i < tope_herramientas; i++){
        if(herramientas[i].tipo == PUERTA){
            posicion_vector = i;    
        }
    }
    return posicion_vector;   
}

int estado_nivel(juego_t juego){
    int estado = 0;
    
    int posicion_puerta = buscar_puerta(juego.niveles[juego.nivel_actual-1].herramientas, juego.niveles[juego.nivel_actual-1].tope_herramientas);
    coordenada_t coord_puerta = juego.niveles[juego.nivel_actual-1].herramientas[posicion_puerta].coordenada;

    if(distancia_manhatann(juego.personaje.posicion, coord_puerta) <= 1 && 
    distancia_manhatann(juego.sombra.posicion, coord_puerta) <= 1 && 
    juego.sombra.esta_viva == true && 
    juego.personaje.vida > 0 && 
    juego.nivel_actual-1 == 0) {
        estado = 1;
    }
    
    if(distancia_manhatann(juego.personaje.posicion, coord_puerta) <= 1 && 
    juego.personaje.tiene_llave == true && 
    distancia_manhatann(juego.sombra.posicion, coord_puerta) <= 1 && 
    juego.sombra.esta_viva == true && 
    juego.personaje.vida > 0 && 
    juego.nivel_actual-1 > 0){
        estado = 1;
    }
            
    return estado;
    
}

/* 
 * Recibe la informacion del juego por referencia y el movimiento hecho. 
 * Si el personaje cae sobre una escalera avanzará un casillero mas
 * En el caso de que la siguiente posicion sea una pared, no avanzará
 * La sombra no puede usar escaleras
 */
void interaccion_escaleras(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_herramientas; i++){
         //escaleras
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].herramientas[i].tipo == ESCALERA){            
            switch(movimiento){
                case IZQUIERDA:
                    if(juego->personaje.posicion.col != 0 && juego->personaje.posicion.col-1 != 0 ){
                        juego->personaje.posicion.col -= 1;
                        if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == true){
                            juego->personaje.posicion.col += 2;
                        }
                        else if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == false){
                            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
                        }
                    }
                    break;
                case DERECHA:
                    if(juego->personaje.posicion.col != MAX_COLUMNAS - 1 && juego->personaje.posicion.col+1 != MAX_COLUMNAS - 1){
                        (juego->personaje.posicion.col) += 1;
                        if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == true){
                            juego->personaje.posicion.col -= 2;
                        }
                        else if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == false){
                            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
                        }
                    }
                    break;
                case ABAJO:
                    if(juego->personaje.posicion.fila != MAX_FILAS - 1 && juego->personaje.posicion.fila+1 != MAX_FILAS - 1){
                        (juego->personaje.posicion.fila) += 1;
                        if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == true){
                            juego->personaje.posicion.fila -= 2;
                        }
                        else if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == false){
                            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
                        }
                    }     
                    break;
                case ARRIBA:
                    if(juego->personaje.posicion.fila != 0 && juego->personaje.posicion.fila-1 != 0){
                        (juego->personaje.posicion.fila) -= 1;
                        if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == true){
                            juego->personaje.posicion.fila += 2;
                        }
                        else if(hay_pared(juego->niveles[juego->nivel_actual-1].paredes, juego->niveles[juego->nivel_actual-1].tope_paredes, juego->personaje.posicion) == false){
                            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
                        }
                    } 
                    break;        
                }
        }
    }
}

/* 
 * Recibe la informacion del juego por referencia y el movimiento hecho. 
 * Si el personaje o la sombra caen sobre una moneda, se le suma una cantidad de puntos entre 10 y 20.
 */
void interaccion_monedas(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_herramientas; i++){
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].herramientas[i].tipo == MONEDA){
            juego->personaje.puntos = juego->personaje.puntos + rand() % 11 + 10;
            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
        }
        if(coordenadas_iguales(juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].herramientas[i].tipo == MONEDA){
            juego->personaje.puntos = juego->personaje.puntos + rand() % 11 + 10;
            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
        }
    }
}

/* 
 * Recibe la informacion del juego por referencia y el movimiento hecho. 
 * Si el personaje cae sobre una llave, cambia el estado de posesion de llave a true
 */
void interaccion_llave(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_herramientas; i++){
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].herramientas[i].tipo == LLAVE){
            juego->personaje.tiene_llave = true;
            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].herramientas, &juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].herramientas[i].coordenada);
        }
    }
}

/* 
 * Recibe la informacion del juego por referencia y el movimiento hecho. 
 * Si la sombra y el personaje estas a menos de un movimiento de la puerta y tienen la llave (menos en el nivel uno), pasan de nivel
 */
void interaccion_puerta(juego_t* juego, char movimiento){
    if(estado_nivel(*juego) == 1 && juego->nivel_actual < 3){
        juego->nivel_actual += 1;
        juego->personaje.interruptor_apretado = false;
        reiniciar_posicioner_personajes(juego);
    }
}

/*
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * El procedimieto analiza la coordenada en la que caerá el personaje o la sombra y de acuerdo a cada tipo de herramienta, realizará una acción correspondiente
 */
void interaccion_herramientas(juego_t* juego, char movimiento){

    interaccion_escaleras(juego, movimiento);
    interaccion_monedas(juego, movimiento);
    interaccion_llave(juego, movimiento);
    interaccion_puerta(juego, movimiento);
}

/*
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * Si el personaje cae en un pozo pierde una vida y es reubicado junto a la sombra
 * la sombra no cae en los pozos
 */
void interaccion_pozos(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_obstaculos; i++){
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == POZO){
            juego->personaje.vida -= 1;
            juego->personaje.posicion = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
            juego->sombra.posicion.fila = juego->personaje.posicion.fila;
            juego->sombra.posicion.col = MAX_COLUMNAS - 1 - juego->personaje.posicion.col;
        }
    }
}
/* 
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * Si el personaje cae sobre el interruptor y cambia su valor a true: cambia en movimiento de la sombra y deja de ser espejado.
 * Ambos se moveran para el mismo lugar
 * Si el valor era true, cambia a false y vuelven a moverse de manera espejada
 */
void interaccion_interruptor(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_obstaculos; i++){
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada) == true && juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == INTERRUPTOR){
            if(juego->personaje.interruptor_apretado == true){
                juego->personaje.interruptor_apretado = false;
            }
            else if(juego->personaje.interruptor_apretado == false){
                juego->personaje.interruptor_apretado = true;
            }
        }
    }
}

/*
 * Recibe la informacion del juego
 * devuelve true si bloo se encuentra dentro de la matriz 3x3 al rededor de la vela
 */
bool esta_dentro_de_matriz(juego_t* juego, int i){

    return(abs(juego->sombra.posicion.fila - juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada.fila) <= 1 && abs(juego->sombra.posicion.col - juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada.col) <= 1);
}

/* 
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * Si la sombra cae en la matriz al rededor de la vela, la cual es de 3x3, muere
 * Si el personaje se para arriba de la sombra la revivie
 * SI el personaje cae sobre la vela, está es eliminada del vector de obstaculos y ya no perjudica a la sombra
 */
void interaccion_vela(juego_t* juego, char movimiento){
    
    if(coordenadas_iguales(juego->sombra.posicion, juego->personaje.posicion) && juego->sombra.esta_viva == false){
        juego->sombra.esta_viva = true;
    }
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_obstaculos; i++){
        
        if(juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == VELA && esta_dentro_de_matriz(juego, i) == true){
            juego->sombra.esta_viva = false;
        }
        if(juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == VELA && juego->sombra.esta_viva == false && coordenadas_iguales(juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada, juego->personaje.posicion)){
            eliminar_herramienta(juego->niveles[juego->nivel_actual-1].obstaculos, &juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada);
        }
        
        if(coordenadas_iguales(juego->sombra.posicion, juego->personaje.posicion) && juego->sombra.esta_viva == false && juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == VELA && esta_dentro_de_matriz(juego, i) == true){
            juego->sombra.esta_viva = false;
            juego->personaje.puntos -= 50;            
        }
    }

    
}

/*
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * Si la sombra cae sobre un portal, intercambia las posiciones con el personaje
 * No sucede nada si el personaje cae sobre un portal
 */
void interaccion_portal(juego_t* juego, char movimiento){
    for(int i = 0; i < juego->niveles[juego->nivel_actual-1].tope_obstaculos; i++){
        if(juego->niveles[juego->nivel_actual-1].obstaculos[i].tipo == PORTAL && coordenadas_iguales(juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].obstaculos[i].coordenada)){
            coordenada_t posicion_aux = juego->personaje.posicion;
            juego->personaje.posicion = juego->sombra.posicion;
            juego->sombra.posicion = posicion_aux;
        }
    }
}

/*
 * Recibe la informacion del juego por referencia y el tipo de movimiento a realizar
 * Analiza la coordenada en la que caerá el personaje o sombra y de acuerdo a cada tipo de obstaculo, realizará una acción correspondiente
 */
void interaccion_obstaculos(juego_t* juego, char movimiento){
    
    interaccion_pozos(juego, movimiento);
    interaccion_interruptor(juego, movimiento);
    interaccion_vela(juego, movimiento);
    interaccion_portal(juego, movimiento);
}


void realizar_jugada(juego_t* juego){

    char movimiento;
    pedir_movimiento(&movimiento);
    es_movimiento_valido(movimiento);

    while((es_movimiento_valido(movimiento)) == false){
        printf("Movimiento no válido. Recordá que los movimientos son A, S, D y W (en mayúscula!) o V para gastar una %s\n", EMOJI_VIDA);
        pedir_movimiento(&movimiento);
    }
    mover_personaje((&juego->personaje), (&juego->sombra), movimiento, juego->personaje.interruptor_apretado);
    interaccion_herramientas(juego, movimiento); 
    interaccion_obstaculos(juego, movimiento);
    
    //paredes    
    for(int k = 0; k < juego->niveles[juego->nivel_actual-1].tope_paredes; k++)  {
        if(coordenadas_iguales(juego->personaje.posicion, juego->niveles[juego->nivel_actual-1].paredes[k]) == true){
            
            switch(movimiento){
                case IZQUIERDA:
                    juego->personaje.posicion.col += 1;
                    break;

                case DERECHA:
                    (juego->personaje.posicion.col) -= 1;
                    break;

                case ABAJO:
                    (juego->personaje.posicion.fila) -= 1;
                    break;

                case ARRIBA:
                    (juego->personaje.posicion.fila) += 1;
                    break;        
            } 
        }    
        if(coordenadas_iguales(juego->sombra.posicion, juego->niveles[juego->nivel_actual-1].paredes[k]) == true){
            switch(movimiento){
                case IZQUIERDA:
                    juego->sombra.posicion.col -= 1;
                    break;

                case DERECHA:
                    (juego->sombra.posicion.col) += 1; 
                    break;

                case ABAJO:
                    (juego->sombra.posicion.fila) -= 1;
                    break;

                case ARRIBA:
                    (juego->sombra.posicion.fila) += 1; 
                    break;        
            } 
        }
    }
    
    imprimir_terreno(*juego);        
            
}

void inicializar_partida(bool configuracion_activa){

    juego_t juego;
    configuracion_t configuracion;
    if(configuracion_activa == false){
        configuracion_default(&configuracion);
    }
    
    else{
        inicializar_configuracion(&configuracion);
    }
    inicializar_juego(&juego, configuracion);      
    imprimir_terreno(juego);
    while(estado_juego(juego) == 0){
        realizar_jugada(&juego);
    }
}






