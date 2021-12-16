#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "src/split.h"

#define RESET "\033[0m"

#define NEGRO "\033[30m"
#define ROJO "\033[31m"
#define VERDE "\033[32m"
#define AMARILLO "\033[33m"
#define BLANCO "\033[37m"
#define GRIS "\033[90m"

#define FONDO_VERDE "\033[42m"


#define BOLD "\033[1m"





typedef struct{
    bool jugando;
    simulador_t* simulador;
}Juego;

bool inicializar_juego(Juego* juego, hospital_t* hospital){
    if(!juego || !hospital)
        return false;

    juego->simulador = simulador_crear(hospital);
    if(!juego->simulador)
        return false;

    juego->jugando = true;
    return true;
}

void destruir_juego(Juego juego){
    simulador_destruir(juego.simulador);
}

char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

int leer_numero(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;

    char** palabras = split(leido, ' ');
    size_t i = 0;
    while(palabras[i] != NULL){
        size_t j = 0;
        bool es_numero = true;
        while(palabras[i][j] != '\0' && es_numero){
            if(!isdigit(palabras[i][j]) && palabras[i][j] != '\n')
                es_numero = false;
            j++;
        }

        if(j == strlen(palabras[i]) && es_numero){
            int numero = atoi(palabras[i]);
            liberar_elementos_split(palabras);
            return numero;
        }

        i++;
    }

    return -1;
}

void mostrar_estadisticas(Juego* juego){
    if(!juego)
        return;

    EstadisticasSimulacion estadisticas;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &estadisticas);

    printf("Las estadisticas del simulador son:\n");
    printf(BOLD "\tCantidad de eventos simulados:\t\t" RESET "%u\n", estadisticas.cantidad_eventos_simulados);
    printf(BOLD "\tCantidad de entrenadores:\t\t" RESET "%u\n", estadisticas.entrenadores_totales);
    printf(BOLD "\tCantidad de entrenadores atendidos:\t" RESET "%u\n", estadisticas.entrenadores_atendidos);
    printf(BOLD "\tCantidad de pokemon:\t\t\t" RESET "%u\n", estadisticas.pokemon_totales);
    printf(BOLD "\tCantidad de pokemon atendidos:\t\t" RESET "%u\n", estadisticas.pokemon_atendidos);
    printf(BOLD "\tCantidad de pokemon en espera:\t\t" RESET "%u\n", estadisticas.pokemon_en_espera);
    printf(BOLD "\tPuntaje actual:\t\t\t\t" RESET  "%u\n" , estadisticas.puntos);
}

void salir_juego(Juego* juego){
    if(!juego)
        return;
    simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
    juego->jugando = false;
}
 

void mostrar_pokemon_en_tratamiento(Juego* juego){
    if(!juego)
        return;

    InformacionPokemon info;
    if(simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &info) == ExitoSimulacion){
        printf("Los datos del pokemon en tratamiento son:\n");
        printf(BOLD "\tNombre: " RESET "%s\n", info.nombre_pokemon);
        printf(BOLD "\tEntrenador: " RESET "%s\n", info.nombre_entrenador);
    }else{
        printf(ROJO "No hay pokemon en tratamiento\n" RESET);
    }
}

void tratar_pokemon(Juego* juego){
    if(!juego)
        return;

    InformacionPokemon pokemon_a_tratar;
    simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &pokemon_a_tratar);
    if(pokemon_a_tratar.nombre_entrenador == NULL && pokemon_a_tratar.nombre_pokemon == NULL){
        printf(ROJO "Ya no quedan pokemon en espera, usa el comando 'p' para atender a otro entrenador\n" RESET);
        return;
    }

    Intento intento;
    printf("Ingresa el nivel del pokemon (mayor a 0):\n");
    int numero_ingresado = leer_numero();
    if(numero_ingresado < 0)
        return;
    intento.nivel_adivinado = (unsigned) numero_ingresado;
    printf("\n");


    simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento);

    if(intento.es_correcto)
        printf(VERDE "%s\n" RESET, intento.resultado_string);
    else
        printf(ROJO "%s\n" RESET, intento.resultado_string);
}

void ver_dificultades(Juego* juego){
    if(!juego)
        return;

    InformacionDificultad informacion;
    informacion.id = 0;
    while(simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &informacion) == ExitoSimulacion){
        if(informacion.en_uso)
            printf(BOLD);

        printf("\t%i) %s ", informacion.id, informacion.nombre_dificultad);

        if(informacion.en_uso)
            printf("(en uso)\n" RESET);
        else
            printf("\n");
        
        informacion.id++;
    }
}

void cambiar_dificultad(Juego* juego){
    if(!juego)
        return;

    ver_dificultades(juego);
    printf("\nEscribi el id de la dificultad a la que quieras cambiar:\n");
    int id = leer_numero();
    if(simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id) == ErrorSimulacion){
        printf("Esa dificultad no existe\n");
        return;
    }
    ver_dificultades(juego);
}

void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case 'q':
            salir_juego(juego);
            break;
        case 'e':
            mostrar_estadisticas(juego);
            break;
        case 'p':
            simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
            printf(GRIS "Atendiendo al proximo entrenador...\n" RESET);
            break;
        case 'i':
            mostrar_pokemon_en_tratamiento(juego);
            break;
        case 'a':
            tratar_pokemon(juego);
            break;
        case 'o':
            ver_dificultades(juego);
            break;
        case 'd':
            cambiar_dificultad(juego);
            break;
        default: 
            break;;
    } 
}

int main(int argc, char *argv[]) {

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    if(!inicializar_juego(&juego, hospital))
        return -1;

    do{
        printf(AMARILLO "> " RESET);
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    }while(juego.jugando);

    destruir_juego(juego);

    return 0;
}
