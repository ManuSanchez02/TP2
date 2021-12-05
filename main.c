#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
    bool jugando;
    simulador_t* simulador;
}Juego;


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

void mostrar_estadisticas(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);

    //Mostrar estadísticas
}


void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case 'q': //Salir del juego
            break;
        case 'e':
            mostrar_estadisticas(juego);
            break;
        default: //el resto de las cosas
            break;
    }
}

int main(int argc, char *argv[]) {

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    inicializar_juego(&juego, hospital);

    do{
        printf("> ");
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    }while(juego.jugando);

    destruir_juego(juego);

    return 0;
}
