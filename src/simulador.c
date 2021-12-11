#include "simulador.h"
#include "hospital_interno.h"
#include "heap.h"
#include "cola.h"

const size_t TAMANIO_INICIAL_HEAP = 6; //! Explicar constante


struct _simulador_t{
    hospital_t* hospital;

};

//-----------------------------------------------------//
/*                FUNCIONES DE EVENTOS                 */
//-----------------------------------------------------//

ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadisticas){
    if(!simulador || !estadisticas)
        return ErrorSimulacion;


    return ExitoSimulacion;
}

ResultadoSimulacion atender_proximo_entrenador(simulador_t* simulador){
    if(!simulador)
        return ErrorSimulacion;


    return ExitoSimulacion;
}


//-----------------------------------------------------//
/*                FUNCIONES AUXILIARES                 */
//-----------------------------------------------------//


int comparador_pokemon(void* pokemon1, void* pokemon2){ // ! Explicar comparacion
    if(!pokemon1 || !pokemon2)
        return 0;

    return (int)pokemon_nivel(pokemon1) - (int)pokemon_nivel(pokemon2);
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


    hospital_destruir(simulador->hospital);
    free(simulador);
}