#include "simulador.h"

struct _simulador_t{
    hospital_t* hospital;
    unsigned pokemon_en_espera;
    unsigned entrenadores_en_espera;
    unsigned puntos;
    unsigned cantidad_eventos_simulados;

};

//-----------------------------------------------------//
/*                FUNCIONES DE EVENTOS                 */
//-----------------------------------------------------//

ResultadoSimulacion obtener_estadisticas(simulador_t* simulador, EstadisticasSimulacion* estadisticas){
    if(!simulador || !estadisticas)
        return ErrorSimulacion;

    estadisticas->entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(simulador->hospital);
    estadisticas->entrenadores_atendidos = estadisticas->entrenadores_totales - simulador->entrenadores_en_espera;
    estadisticas->pokemon_totales = (unsigned)hospital_cantidad_pokemon(simulador->hospital);
    estadisticas->pokemon_en_espera = simulador->pokemon_en_espera;
    estadisticas->pokemon_atendidos = estadisticas->pokemon_totales - estadisticas->pokemon_en_espera;
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;
    estadisticas->puntos = simulador->puntos;

    return ExitoSimulacion;
}

ResultadoSimulacion atender_proximo_entrenador(simulador_t* simulador){
    if(!simulador)
        return ErrorSimulacion;


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