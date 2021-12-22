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

#define BOLD "\033[1m"

#define TECLA_MOSTRAR_ESTADISTICAS 'e'
#define TECLA_ATENDER_ENTERNADOR 'p'
#define TECLA_MOSTRAR_POKEMON 'i'
#define TECLA_TRATAR_POKEMON 'a'
#define TECLA_VER_DIFICULTADES 'o'
#define TECLA_CAMBIAR_DIFICULTAD 'd'
#define TECLA_LIMPIAR_PANTALLA 'c'
#define TECLA_MOSTRAR_AYUDA 'h'
#define TECLA_SALIR 'q'

typedef struct{
    bool jugando;
    simulador_t* simulador;
}Juego;

//--------------------------------//
//     FUNCIONES DIFICULTADES     //
//--------------------------------//

/*
 * Pre: 
 * Post: devuelve un la diferencia entre el nivel_adivinado y el nivel_pokemon
 */
int verificar_nivel_nuevo(unsigned int nivel_adivinado, unsigned int nivel_pokemon){
    return (int)(nivel_adivinado - nivel_pokemon);
}

/*
 * Pre: 
 * Post: devuelve el puntaje respectivo a la dificultad imposible
 */
unsigned int calcular_puntaje_imposible(unsigned int intentos){
    if(intentos == 0)
        return 1000;

    if(intentos <= 100)
        return 1000-10*(intentos-1);

    return 0;
}

/*
 * Pre: resultado_verificacion debe ser el valor de retorno de verificar_nivel_nuevo
 * Post: devuelve el string resultado respectivo de la dificultad imposible
 */
const char* verificacion_a_string_imposible(int resultado_verificacion){
    if(resultado_verificacion == 0)
        return "Correcto!";
    return "No";
}

/*
 * Pre: 
 * Post: devuelve el puntaje respectivo a la dificultad espacio
 */
unsigned int calcular_puntaje_espacio(unsigned int intentos){
    if(intentos >= 5)
        return 0;
    return 100;
}

/*
 * Pre: resultado_verificacion debe ser el valor de retorno de verificar_nivel_nuevo
 * Post: devuelve el string resultado respectivo de la dificultad espacio
 */
const char* verificacion_a_string_espacio(int resultado_verificacion){
    int distancia = abs(resultado_verificacion);

    if(distancia == 0)
        return "Llegaste a la tierra!";
    else if(distancia <= 10)
        return "Estas en la atmosfera terrestre";
    else if(distancia <= 25)
        return "Estas pasando por la luna";
    else if(distancia <= 50)
        return "Estas entrando en el sistema solar";
    else if(distancia <= 80)   
        return "Estas entrando en la via lactea";
    else
        return "Estas explorando el universo";
}

//-----------------------------//
//       FUNCIONES JUEGO       //
//-----------------------------//

/*
 * Pre: -
 * Post: crea una dificultad dados los parametros y la agrega al simulador
 */
bool agregar_nueva_dificultad(simulador_t* simulador, const char* nombre, unsigned int (*calcular_puntaje)(unsigned int), int (*verificar_nivel)(unsigned int, unsigned int), const char *(verificacion_a_string)(int)){
    if(!simulador || !nombre || !calcular_puntaje || !verificar_nivel || !verificacion_a_string){
        return false;
    }
    
    DatosDificultad dificultad;
    dificultad.nombre = nombre;
    dificultad.calcular_puntaje = calcular_puntaje;
    dificultad.verificar_nivel = verificar_nivel;
    dificultad.verificacion_a_string = verificacion_a_string;

    return simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ExitoSimulacion;
}

/*
 * Pre: -
 * Post: inicializa el juego reservando memoria para todas sus estructuras y agregando las dificultades nuevas.
 *       Devuelve true si se pudo inicializar el juego, false en caso contrario. Si falla, libera la memoria reservada.
 */
bool inicializar_juego(Juego* juego, hospital_t* hospital){
    if(!juego || !hospital)
        return false;

    juego->simulador = simulador_crear(hospital);
    if(!juego->simulador)
        return false;

    if(!agregar_nueva_dificultad(juego->simulador, "Imposible", calcular_puntaje_imposible, verificar_nivel_nuevo, verificacion_a_string_imposible) || !agregar_nueva_dificultad(juego->simulador, "Espacial", calcular_puntaje_espacio, verificar_nivel_nuevo, verificacion_a_string_espacio)){
        simulador_destruir(juego->simulador);
        return false;
    }

    juego->jugando = true;
    return true;
}

/*
 * Pre: -
 * Post: libera la memoria reservada para el juego
 */
void destruir_juego(Juego juego){
    simulador_destruir(juego.simulador);
}

/*
 * Pre: -
 * Post: lee el comando ingresado por el usuario y lo devuelve en minusculas
 */
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

/*
 * Pre: -
 * Post: lee el primer numero ingresado por el usuario y lo devuelve. Si no se ingreso ningun numero, devuelve -1.
 */
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

/*
 * Pre: -
 * Post: imprime por pantalla las estadisticas del juego
 */
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

/*
 * Pre: -
 * Post: finaliza la simulacion y termina el juego
 */
void salir_juego(Juego* juego){
    if(!juego)
        return;
    simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
    juego->jugando = false;
}
 
/*
* Pre: -
* Post: imprime por pantalla el pokemon en tratamiento
*/
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

/*
* Pre: -
* Post: atiende al proximo entrenador e imprime por pantalla si se pudo atender o no.
*/
void atender_entrenador(Juego* juego){
    if(!juego)
        return;

    if(simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion){
        printf(GRIS "Atendiendo al proximo entrenador...\n" RESET);
    }else{
        printf(ROJO "No hay entrenadores en espera\n" RESET);
    }
}

/*
* Pre: -
* Post: pide al usuario que ingrese el nivel del pokemon, y lo compara con el nivel del pokemon en tratamiento. Si acierta, se pone en
*       tratamiento al siguiente pokemon y se añaden los puntos correspondientes. Tambien imprime por pantalla el string resultado de la dificultad seleccionada
*/
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

/*
* Pre: -
* Post: imprime por pantalla las dificultades
*/
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

/*
* Pre: -
* Post: imprime por pantalla las dificultades y pide al usuario que ingrese el id de una dificultad valida. Si lo ingresa, se cambia la dificultad a esta
*/
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

/*
* Pre: -
* Post: imprime por pantalla los comandos disponibles
*/
void mostrar_comandos(){
    printf(VERDE "Los comandos disponibles son:\n" RESET);
    printf(BOLD "\t'%c'\t" RESET "Mostrar estadisticas\n", TECLA_MOSTRAR_ESTADISTICAS);
    printf(BOLD "\t'%c'\t" RESET "Atender al proximo entrenador\n", TECLA_ATENDER_ENTERNADOR);
    printf(BOLD "\t'%c'\t" RESET "Mostrar pokemon en tratamiento\n", TECLA_MOSTRAR_POKEMON);
    printf(BOLD "\t'%c'\t" RESET "Tratar pokemon\n", TECLA_TRATAR_POKEMON);
    printf(BOLD "\t'%c'\t" RESET "Ver dificultades\n", TECLA_VER_DIFICULTADES);
    printf(BOLD "\t'%c'\t" RESET "Cambiar dificultad\n", TECLA_CAMBIAR_DIFICULTAD);
    printf(BOLD "\t'%c'\t" RESET "Limpiar la pantalla\n", TECLA_LIMPIAR_PANTALLA);
    printf(BOLD "\t'%c'\t" RESET "Mostrar esta pantalla\n", TECLA_MOSTRAR_AYUDA);
    printf(BOLD "\t'%c'\t" RESET "Salir del juego\n", TECLA_SALIR);
}

/*
* Pre: -
* Post: ejecuta un comando segun la tecla ingresada
*/
void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case TECLA_SALIR:
            salir_juego(juego);
            break;
        case TECLA_MOSTRAR_ESTADISTICAS:
            mostrar_estadisticas(juego);
            break;
        case TECLA_ATENDER_ENTERNADOR:
            atender_entrenador(juego);
            break;
        case TECLA_MOSTRAR_POKEMON:
            mostrar_pokemon_en_tratamiento(juego);
            break;
        case TECLA_TRATAR_POKEMON:
            tratar_pokemon(juego);
            break;
        case TECLA_VER_DIFICULTADES:
            ver_dificultades(juego);
            break;
        case TECLA_CAMBIAR_DIFICULTAD:
            cambiar_dificultad(juego);
            break;
        case TECLA_LIMPIAR_PANTALLA:
            system("clear");
            break;
        case TECLA_MOSTRAR_AYUDA:
            mostrar_comandos();
            break;
        default: 
            break;
    } 
}

int main(int argc, char *argv[]) {
    system("clear");
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    if(!inicializar_juego(&juego, hospital))
        return -1;

    printf("Escribi un comando o '%c' para ver los comandos disponibles:\n", TECLA_MOSTRAR_AYUDA);

    do{
        printf(AMARILLO "> " RESET);
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    }while(juego.jugando);

    destruir_juego(juego);

    return 0;
}
