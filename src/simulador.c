#include "simulador.h"

struct _simulador_t{
    hospital_t* hospital;
};


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

    switch(evento){
        case ObtenerEstadisticas:
            break;

        case AtenderProximoEntrenador:
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

    return ExitoSimulacion;
}



void simulador_destruir(simulador_t* simulador){
    if(!simulador)
        return;

    hospital_destruir(simulador->hospital);
    free(simulador);
}