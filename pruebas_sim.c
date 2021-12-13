#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>


/* Pruebas */
void dadoUnHospital_puedoCrearYDestruirUnSimulador(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital");
    simulador_t* simulador = simulador_crear(h);

    pa2m_afirmar((simulador != NULL), "El simulador se pudo crear con exito");

    simulador_destruir(simulador);
}

void dadoUnHospitalNULL_noPuedoCrearUnSimulador(){
    hospital_t* h = NULL;
    simulador_t* simulador = simulador_crear(h);

    pa2m_afirmar((simulador == NULL), "El simulador con hospital NULL no se pudo crear");
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoObtenerEstadisticas(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion* estadisticas = malloc(sizeof(EstadisticasSimulacion));
    

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerEstadisticas, estadisticas) == ExitoSimulacion), "Puedo obtener las estadisticas");
    pa2m_afirmar((estadisticas->cantidad_eventos_simulados == 0), "La cantidad de eventos simulados es 0");
    pa2m_afirmar((estadisticas->entrenadores_atendidos == 0), "La cantidad de entrenadores atendidos es 0");
    pa2m_afirmar((estadisticas->entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas->pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas->pokemon_en_espera == hospital_cantidad_pokemon(h)), "La cantidad de pokemon en espera es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas->pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas->puntos == 0), "La cantidad de puntos es 0");


    simulador_destruir(simulador);
    free(estadisticas);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderAlProximoEntrenador(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);

    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Puedo atender un entrenador");
    

    simulador_destruir(simulador);
}


//----------------------------------------------------------------------------------------------//

int main(){

    pa2m_nuevo_grupo("Pruebas de creacion y destruccion");
    dadoUnHospital_puedoCrearYDestruirUnSimulador();
    dadoUnHospitalNULL_noPuedoCrearUnSimulador();

    pa2m_nuevo_grupo("Pruebas de evento OBTENER");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoObtenerEstadisticas();

    pa2m_nuevo_grupo("Pruebas de evento ATENDER PROXIMO ENTRENADOR");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderAlProximoEntrenador();




    return pa2m_mostrar_reporte();
}
