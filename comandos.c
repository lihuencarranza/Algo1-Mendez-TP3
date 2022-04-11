#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include"comandos.h"
#include"la_sombra_de_mac.h"

#define CANTIDAD_LEIDOS 5
#define MAX_USER 100
#define MAX_BOOL 5
#define LECTURA "%[^;];%i;%i;%i;%[^\n]\n" 
#define ESCRITURA "%s;%i;%i;%i;%s\n"
#define LECTURA_CONFG "N%i_%[^=]=%i\n"
#define FALLO -1
#define EXITO 0


/*
* Recibe una partida recién creada sin paramteros inicializados
* Solicita al usuario los datos para llenar el struct de la partida a agregar
* Suponemos que los valores ingresados por el ususario son válidos
*/
void solicitar_datos(partida_t* partida_a_agregar){

    int gano;
    printf("Comenzaremos a llenar los datos necesarios\n");
    printf("Ingresar nombre (Con mayúscula en la primera letra y espacios de ser un nombre compuesto):");
    scanf("%s", partida_a_agregar->jugador);
    
    printf("Nivel llegado:");
    scanf("%i", &partida_a_agregar->nivel_llegado);
    
    
    printf("Puntos obtenidos:");
    scanf("%i", &partida_a_agregar->puntos);
    
    printf("Vidas restantes:");
    scanf("%i", &partida_a_agregar->vidas_restantes);
    
    printf("Ganó la partida? (Ingresar '0' si ganó o '-1' si perdió):");
    scanf("%i", &gano);
    
    if(gano == 0){
        partida_a_agregar->gano = true;
    }
    else if(gano == -1){
        partida_a_agregar->gano = false;
        
    } 
}


int agregar_partida(char* archivo, partida_t partida_a_agregar){

    FILE* arch = fopen(archivo, "r");
    if(!arch){
        perror("No se pudo abrir el archivo original\n");
        return FALLO;
    }
    FILE* arch_aux = fopen("auxiliar.csv", "w");
    if(!arch_aux){
        perror("No se pudo abrir el archivo auxiliar\n");
        fclose(arch);
        return FALLO;
    }
    char bool_agregada[MAX_BOOL];  
    solicitar_datos(&partida_a_agregar);
    if(partida_a_agregar.gano == true){
        strcpy(bool_agregada, "Si");
    }
    else if(partida_a_agregar.gano == false){
        strcpy(bool_agregada, "No");
    }
    
    partida_t partida;
    char gano_leida[MAX_BOOL]; 

    int llenado = fscanf(arch, LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_leida);
    if(strcmp(gano_leida, "Si") == 0){
        partida.gano =  true;
    }
    else if(strcmp(gano_leida, "No") == 0){
        partida.gano = false;
    }
    
    bool agregada = false;
    while(llenado == CANTIDAD_LEIDOS){


        if(strcmp(partida.jugador, partida_a_agregar.jugador) >= 0 && !agregada){
            fprintf(arch_aux, ESCRITURA, partida_a_agregar.jugador, partida_a_agregar.nivel_llegado, partida_a_agregar.puntos, partida_a_agregar.vidas_restantes, bool_agregada);
            agregada = true;
            printf("agregada\n");
        }
        
        fprintf(arch_aux, ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano_leida);  
            
        llenado = fscanf(arch, LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_leida); 
       
    }
    if(agregada == false){
        fprintf(arch_aux, ESCRITURA, partida_a_agregar.jugador, partida_a_agregar.nivel_llegado, partida_a_agregar.puntos, partida_a_agregar.vidas_restantes, bool_agregada);
    }
    rename("auxiliar.csv", archivo);
    fclose(arch);
    fclose(arch_aux);
    return EXITO;
   
}

int eliminar_partida(char* archivo, char jugador[MAX_NOMBRE]){

    FILE* arch = fopen(archivo, "r");
    if(!arch){
        perror("No se pudo abrir el archivo original\n");
        return FALLO;
    }
    
    FILE* arch_aux = fopen("auxiliar.csv", "w");
    if(!arch_aux){
        perror("No se pudo abrir el archivo auxiliar\n");
        fclose(arch);
        return FALLO;
    }

    partida_t partida;
    char gano_leida[MAX_BOOL];
    int lleno;
    
    bool eliminada = false;
    
    lleno = fscanf(arch, LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_leida);
    if(strcmp(gano_leida, "Si") == 0){
        partida.gano =  true;
    }
    else if(strcmp(gano_leida, "No") == 0){
        partida.gano = false;
    }
    
    while(lleno == CANTIDAD_LEIDOS){
        if(strcmp(jugador, partida.jugador) != 0 || (strcmp(jugador, partida.jugador) == 0 && eliminada)){
            fprintf(arch_aux, ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano_leida);
        }else{
            eliminada = true;
            printf("eliminada\n");
        }
        lleno = fscanf(arch, LECTURA, partida.jugador, &partida.nivel_llegado, &partida.puntos, &partida.vidas_restantes, gano_leida);
        if(strcmp(gano_leida, "Si") == 0){
            partida.gano =  true;
        }
        else if(strcmp(gano_leida, "No") == 0){
            partida.gano = false;
        }
    }


    rename("auxiliar.csv", archivo);
    fclose(arch);
    fclose(arch_aux);
    return EXITO;
}

/*
* Recibe el vector de partidas copiadas del archivo original, su tope, un char que guarda un string correspondiente al bool del formato, el archivo auxiliar
* Imprime las partidas ya ordenadas en el archivo auxiliar
*/
void imprimir_partidas_en_archivo(partida_t partidas[MAX_PARTIDAS], int tope_partidas, char gano_leida[MAX_BOOL], FILE* arch_aux){
    for(int x = 0; x < tope_partidas; x++){
        fprintf(arch_aux, ESCRITURA, partidas[x].jugador, partidas[x].nivel_llegado, partidas[x].puntos, partidas[x].vidas_restantes, gano_leida);
        if(strcmp(gano_leida, "Si") == 0){
            partidas[x].gano =  true;
        }
        else if(strcmp(gano_leida, "No") == 0){
            partidas[x].gano = false;
        }
    }
}

/*
* Recibe las partidas registradas del archivo original y su tope
* Las ordena de acuerdo al método de burbujeo
*/
void ordenar_partidas(partida_t partidas[MAX_PARTIDAS], int tope_partidas){
    char aux[MAX_NOMBRE];
    for(int i = 1; i < tope_partidas; i++){
        for(int j = 0; j < tope_partidas - i; j++){
            if(strcmp(partidas[j].jugador, partidas[j+1].jugador) > 0){
                strcpy(aux, partidas[j].jugador);
                strcpy(partidas[j].jugador, partidas[j+1].jugador);
                strcpy(partidas[j+1].jugador, aux);
            }
        }
    }
}

int procedimiento_ordenar_partidas(char* archivo){
    FILE* arch = fopen(archivo, "r");
    if(!arch){
        perror("No se pudo abrir el archivo original\n");
        return FALLO;
    }
    
    FILE* arch_aux = fopen("auxiliar.csv", "w");
    if(!arch_aux){
        perror("No se pudo abrir el archivo auxiliar\n");
        fclose(arch);
        return FALLO;
    }

    char gano_leida[MAX_BOOL];
    partida_t partidas[MAX_PARTIDAS];
    int tope_partidas = 0;
    
    
    int leidos = fscanf(arch, LECTURA, partidas[tope_partidas].jugador, &partidas[tope_partidas].nivel_llegado, &partidas[tope_partidas].puntos, &partidas[tope_partidas].vidas_restantes, gano_leida);
    if(strcmp(gano_leida, "Si") == 0){
        partidas[tope_partidas].gano =  true;
    }
    else if(strcmp(gano_leida, "No") == 0){
        partidas[tope_partidas].gano = false;
    }
   
    while(leidos == CANTIDAD_LEIDOS){
        tope_partidas++;
        
        leidos = fscanf(arch, LECTURA, partidas[tope_partidas].jugador, &partidas[tope_partidas].nivel_llegado, &partidas[tope_partidas].puntos, &partidas[tope_partidas].vidas_restantes, gano_leida);
        if(strcmp(gano_leida, "Si") == 0){
            partidas[tope_partidas].gano =  true;
        }
        else if(strcmp(gano_leida, "No") == 0){
            partidas[tope_partidas].gano = false;
        }
    }

    ordenar_partidas(partidas, tope_partidas);
    
    imprimir_partidas_en_archivo(partidas, tope_partidas, gano_leida, arch_aux);

    

    printf("Partidas ordenadas\n");
    rename("auxiliar.csv", archivo);
    fclose(arch);
    fclose(arch_aux);
    return EXITO;
}





