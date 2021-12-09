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

int main(){

    pa2m_nuevo_grupo("Pruebas de creacion y destruccion");
    dadoUnHospital_puedoCrearYDestruirUnSimulador();
    dadoUnHospitalNULL_noPuedoCrearUnSimulador();



    return pa2m_mostrar_reporte();
}
