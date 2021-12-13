#include "simulador.h"
#include "hospital_interno.h"
#include "heap.h"
#include "cola.h"
#include <string.h>

const size_t TAMANIO_INICIAL_HEAP = 6; //! Explicar constante


struct _simulador_t{
    hospital_t* hospital;
    cola_t* entrenadores_en_espera;
    heap_t* pokemon_atendidos;
    unsigned cantidad_entrenadores_totales;
    unsigned puntos;
    unsigned cantidad_eventos_simulados;
    unsigned cantidad_pokemon_atendidos;
    pokemon_t* pokemon_en_tratamiento;
};

typedef struct datos_atender_pokemon_entrenador{
    heap_t* pokemon_atendidos;
    entrenador_t* entrenador;
    unsigned* cantidad_pokemon_atendidos;
}datos_atender_pokemon_entrenador_t;


//-----------------------------------------------------//
/*                FUNCIONES AUXILIARES                 */
//-----------------------------------------------------//


int comparador_pokemon(void* pokemon1, void* pokemon2){ // ! Explicar comparacion
    if(!pokemon1 || !pokemon2)
        return 0;

    return (int)pokemon_nivel(pokemon1) - (int)pokemon_nivel(pokemon2);
}


/*
 * Pre: pokemon_a_atender debe ser un puntero a pokemon y datos aux debe ser un puntero a un datos_atender_pokemon_entrenador_t inicializado con los datos correspondientes
 * Post: Devuelve false si hubo un error a la hora de insertar y true en caso contrario. Inserta el pokemon a atender en el heap de datos_aux->pokemon_atendidos
 */
bool atender_pokemon_de_entrenador(void* pokemon_a_atender, void* _datos_aux){
    if(!pokemon_a_atender || !_datos_aux)
        return false;

    datos_atender_pokemon_entrenador_t* datos_aux = _datos_aux;

    if(pokemon_entrenador(pokemon_a_atender) == datos_aux->entrenador){
        (*datos_aux->cantidad_pokemon_atendidos)++;
        return heap_insertar(datos_aux->pokemon_atendidos, pokemon_a_atender) != NULL;
    }


    return true;
}

/*
 * Pre: entrenador_a_encolar debe ser un puntero a entrenador, cola_entrenadores un puntero a una cola de entrenadores
 * Post: Devuelve true si pudo encolar entrenador_a_encolar en cola_entrenadores y false en caso contrario.
 */
bool encolar_entrenadores(void* entrenador_a_encolar, void* cola_entrenadores){
    if(!cola_entrenadores || !entrenador_a_encolar)
        return false;

    return cola_encolar(cola_entrenadores, entrenador_a_encolar) != NULL;
}

//-----------------------------------------------------//
/*                FUNCIONES DE EVENTOS                 */
//-----------------------------------------------------//

/*
 * Pre: 
 * Post: Llena los campos de estadisticas con los datos del simulador. Si tiene exito, devuelve ExitoSimulacion y en caso contrario, ErrorSimulacion
 */
ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadisticas){
    if(!simulador || !estadisticas)
        return ErrorSimulacion;

    estadisticas->entrenadores_totales = (unsigned) lista_tamanio(simulador->hospital->lista_entrenadores);
    estadisticas->entrenadores_atendidos = estadisticas->entrenadores_totales - (unsigned) cola_tamanio(simulador->entrenadores_en_espera);
    estadisticas->pokemon_atendidos = simulador->cantidad_pokemon_atendidos;
    estadisticas->pokemon_totales = (unsigned) abb_tamanio(simulador->hospital->lista_pokemon);
    estadisticas->pokemon_en_espera = estadisticas->pokemon_totales - estadisticas->pokemon_atendidos;
    estadisticas->puntos = simulador->puntos;
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;

    return ExitoSimulacion;
}


/*
 * Pre: 
 * Post: Agrega los pokemon del siguiente entrenador en la sala de espera a los pokemones atendidos. Si es exitoso devuelve ExitoSimulacion. 
 *       Si falla o no quedan entrenadores en la cola, devuelve ErrorSimulacion.
 */
ResultadoSimulacion atender_proximo_entrenador(simulador_t* simulador){
    if(!simulador || cola_vacia(simulador->entrenadores_en_espera))
        return ErrorSimulacion;

    entrenador_t* entrenador_atendido = cola_desencolar(simulador->entrenadores_en_espera);

    datos_atender_pokemon_entrenador_t datos_aux;
    datos_aux.entrenador = entrenador_atendido;
    datos_aux.pokemon_atendidos = simulador->pokemon_atendidos;
    datos_aux.cantidad_pokemon_atendidos = &(simulador->cantidad_pokemon_atendidos);

    
    if(abb_con_cada_elemento(simulador->hospital->lista_pokemon, INORDEN, atender_pokemon_de_entrenador, &datos_aux) != abb_tamanio(simulador->hospital->lista_pokemon))
        return ErrorSimulacion;
    
    if(!simulador->pokemon_en_tratamiento)
        simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->pokemon_atendidos);
        
    return ExitoSimulacion;
}


ResultadoSimulacion obtener_informacion_pokemon_en_tratamiento(simulador_t* simulador, InformacionPokemon* info_pokemon){
    if(!simulador || !info_pokemon)
        return ErrorSimulacion;

    if(!simulador->pokemon_en_tratamiento){
        info_pokemon->nombre_entrenador = NULL;
        info_pokemon->nombre_pokemon = NULL;
        return ErrorSimulacion;
    }

    info_pokemon->nombre_entrenador = pokemon_entrenador(simulador->pokemon_en_tratamiento)->nombre;
    info_pokemon->nombre_pokemon = simulador->pokemon_en_tratamiento->nombre;
    return ExitoSimulacion;
}

//-----------------------------------------------------//
/*            PRIMITIVAS DE SIMULACION                 */
//-----------------------------------------------------//

simulador_t* simulador_crear(hospital_t* hospital){
    if(!hospital)
        return NULL;
    
    simulador_t* simulador = malloc(sizeof(simulador_t));
    if(!simulador)
        return NULL;
   
    simulador->hospital = hospital;
    simulador->entrenadores_en_espera = cola_crear();
    simulador->pokemon_atendidos = heap_crear(TAMANIO_INICIAL_HEAP, comparador_pokemon);
    if(!simulador->entrenadores_en_espera || !simulador->pokemon_atendidos){
        heap_destruir(simulador->pokemon_atendidos);
        cola_destruir(simulador->entrenadores_en_espera);
        free(simulador);
        return NULL;
    }

    lista_con_cada_elemento(simulador->hospital->lista_entrenadores, encolar_entrenadores, simulador->entrenadores_en_espera);

    simulador->cantidad_entrenadores_totales = (unsigned) cola_tamanio(simulador->entrenadores_en_espera);
    simulador->pokemon_en_tratamiento = NULL;
    simulador->cantidad_eventos_simulados = 0;
    simulador->cantidad_pokemon_atendidos = 0;
    simulador->puntos = 0;
    
    return simulador;
}


ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador)
        return ErrorSimulacion;

    ResultadoSimulacion resultado = ErrorSimulacion;
    simulador->cantidad_eventos_simulados++;

    switch(evento){
        case ObtenerEstadisticas:
            resultado = obtener_estadisticas(simulador, datos);
            break;

        case AtenderProximoEntrenador:
            resultado = atender_proximo_entrenador(simulador);
            break;

        case ObtenerInformacionPokemonEnTratamiento:
            resultado = obtener_informacion_pokemon_en_tratamiento(simulador, datos);
            break;

        case AdivinarNivelPokemon:
            break;

        case AgregarDificultad:
            break;

        case SeleccionarDificultad:
            break;

        case ObtenerInformacionDificultad:
            break;

        case FinalizarSimulacion:
            break;
        
        default: 
            return ErrorSimulacion;
    }

    return resultado;
}



void simulador_destruir(simulador_t* simulador){
    if(!simulador)
        return;

    heap_destruir(simulador->pokemon_atendidos);
    cola_destruir(simulador->entrenadores_en_espera);
    hospital_destruir(simulador->hospital);
    free(simulador);
}