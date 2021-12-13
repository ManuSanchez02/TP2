#include "simulador.h"
#include "hospital_interno.h"
#include "heap.h"
#include "cola.h"

const size_t TAMANIO_INICIAL_HEAP = 6; //! Explicar constante


struct _simulador_t{
    hospital_t* hospital;
    cola_t* entrenadores_en_espera;
    heap_t* pokemon_atendidos;
    unsigned cantidad_entrenadores_totales;
    unsigned puntos;
    unsigned cantidad_eventos_simulados;
    pokemon_t* pokemon_en_tratamiento;
};

typedef struct pokemon_y_entrenador{
    pokemon_t* pokemon;
    entrenador_t* entrenador;
}pokemon_y_entrenador_t;


//-----------------------------------------------------//
/*                FUNCIONES AUXILIARES                 */
//-----------------------------------------------------//


int comparador_pokemon(void* pokemon1, void* pokemon2){ // ! Explicar comparacion
    if(!pokemon1 || !pokemon2)
        return 0;

    return (int)pokemon_nivel(pokemon1) - (int)pokemon_nivel(pokemon2);
}

/*
 * Pre: entrenador debe ser un puntero a entrenador, _pokemon_atendidos un puntero al heap de pokemones y _pokemon_a_atender un puntero a pokemon
 * Post: Devuelve el heap de pokemon si la operacion fue exitosa y NULL en caso contrario. Solo agrega al pokemon si su entrenador es igual a entrenador.
 */
void* atender_pokemon(void* _pokemon_atendidos, void* _pokemon_a_atender, void* entrenador){
    if(!_pokemon_atendidos || !_pokemon_a_atender || !entrenador)
        return NULL;

    heap_t* pokemon_atendidos = _pokemon_atendidos;
    pokemon_t* pokemon_a_atender = _pokemon_a_atender;

    if(pokemon_entrenador(pokemon_a_atender) == entrenador)
        return heap_insertar(pokemon_atendidos, pokemon_a_atender);

    return pokemon_atendidos;
}

void* encolar_entrenadores(void* cola_entrenadores, void* entrenador_a_encolar, void* aux){
    if(!cola_entrenadores || !entrenador_a_encolar)
        return NULL;

    return cola_encolar(cola_entrenadores, entrenador_a_encolar);
}

//-----------------------------------------------------//
/*                FUNCIONES DE EVENTOS                 */
//-----------------------------------------------------//

ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadisticas){
    if(!simulador || !estadisticas)
        return ErrorSimulacion;

    estadisticas->entrenadores_totales = (unsigned) lista_tamanio(simulador->hospital->lista_entrenadores);
    estadisticas->entrenadores_atendidos = estadisticas->entrenadores_totales - (unsigned) cola_tamanio(simulador->entrenadores_en_espera);
    estadisticas->pokemon_atendidos = (unsigned) heap_tamanio(simulador->pokemon_atendidos);
    estadisticas->pokemon_totales = (unsigned) lista_tamanio(simulador->hospital->lista_pokemon);
    estadisticas->pokemon_en_espera = estadisticas->pokemon_totales - estadisticas->pokemon_atendidos;
    estadisticas->puntos = simulador->puntos;
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;

    return ExitoSimulacion;
}

ResultadoSimulacion atender_proximo_entrenador(simulador_t* simulador){
    if(!simulador || cola_vacia(simulador->entrenadores_en_espera))
        return ErrorSimulacion;

    entrenador_t* entrenador_atendido = cola_desencolar(simulador->entrenadores_en_espera);
    
    if(!lista_copiar_a(simulador->hospital->lista_pokemon, atender_pokemon, simulador->pokemon_atendidos, entrenador_atendido))
        return ErrorSimulacion;
    
    if(!simulador->pokemon_en_tratamiento)
        simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->pokemon_atendidos);
    
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

    lista_copiar_a(simulador->hospital->lista_entrenadores, encolar_entrenadores, simulador->entrenadores_en_espera, NULL);

    simulador->cantidad_entrenadores_totales = (unsigned) cola_tamanio(simulador->entrenadores_en_espera);
    simulador->pokemon_en_tratamiento = NULL;
    simulador->cantidad_eventos_simulados = 0;
    simulador->puntos = 0;
    
    return simulador;
}


ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador)
        return ErrorSimulacion;

    ResultadoSimulacion resultado = ErrorSimulacion;

    switch(evento){
        case ObtenerEstadisticas:
            resultado = obtener_estadisticas(simulador, datos);
            break;

        case AtenderProximoEntrenador:
            resultado = atender_proximo_entrenador(simulador);
            break;

        case ObtenerInformacionPokemonEnTratamiento:
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