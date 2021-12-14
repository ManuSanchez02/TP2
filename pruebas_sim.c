#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <string.h>


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

void dadoUnSimuladorNULL_noPuedoHacerOperaciones(){
    simulador_t* simulador = NULL;
    EstadisticasSimulacion estadisticas;
    InformacionPokemon info_pokemon;


    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas) == ErrorSimulacion), "No se puede obtener estadisticas de un simulador NULL");
    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion), "No se puede atender proximo entrenador de un simulador NULL");
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &info_pokemon) == ErrorSimulacion), "No se puede obtener informacion del pokemon en tratamiento de un simulador NULL");
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoObtenerEstadisticas(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;
    

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas) == ExitoSimulacion), "Puedo obtener las estadisticas");
    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 1), "La cantidad de eventos simulados es 1");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 0), "La cantidad de entrenadores atendidos es 0");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)), "La cantidad de pokemon en espera es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");


    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_sinEstructuraDeEstadisticas_noPuedoObtenerEstadisticas(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion* estadisticas = NULL;
    
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerEstadisticas, estadisticas) == ErrorSimulacion), "No puedo obtener las estadisticas sin dato");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderAlProximoEntrenador(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;


    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Puedo atender un entrenador");

    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 2), "La cantidad de eventos simulados es 2");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 1), "La cantidad de entrenadores atendidos es 1");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 4), "La cantidad de pokemon atendidos es 4");
    pa2m_afirmar((estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)-4), "La cantidad de pokemon en espera es igual a la cantidad total del hospital menos la cantidad atendida");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_ySinEntrenadoresEnEspera_noPuedoAtenderAlProximoEntrenador(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;


    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion), "No puedo atender un entrenador ya que no hay");
    
    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 2), "La cantidad de eventos simulados es 2");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 0), "La cantidad de entrenadores atendidos es 0");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)), "La cantidad de pokemon en espera es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderVariosEntrenadores(){
        hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;


    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Puedo atender un primer entrenador");
    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Puedo atender un segundo entrenador");
    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Puedo atender un tercer entrenador");

    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 4), "La cantidad de eventos simulados es 4");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 3), "La cantidad de entrenadores atendidos es 3");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 13), "La cantidad de pokemon atendidos es 13");
    pa2m_afirmar((estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)-13), "La cantidad de pokemon en espera es igual a la cantidad total del hospital menos la cantidad atendida");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_noPuedoObtenerLaInformacionDelPokemonEnTratamiento(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionPokemon informacion;

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion) == ErrorSimulacion), "No puedo obtener la informacion del pokemon en tratamiento");

    pa2m_afirmar((informacion.nombre_entrenador == NULL), "El nombre del pokemon es NULL");
    pa2m_afirmar((informacion.nombre_entrenador == NULL), "El nombre del entrenador es NULL");
   

    simulador_destruir(simulador); 
}

void dadoUnSimuladorConUnEntrenadorAtendido_puedoObtenerLaInformacionDelPokemonEnTratamiento(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionPokemon informacion;

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &informacion) == ExitoSimulacion), "Puedo obtener la informacion del pokemon en tratamiento");

    pa2m_afirmar((strcmp(informacion.nombre_pokemon, "rampardos") == 0), "El nombre del pokemon es correcto");
    pa2m_afirmar((strcmp(informacion.nombre_entrenador, "lucas") == 0), "El nombre del entrenador es correcto");
   

    simulador_destruir(simulador);
}

/*
 * Devuelve el puntaje respectivo a la dificultad facil
 */
unsigned int puntaje_personalizado(const unsigned int intentos){
    if(intentos > 10)
        return 0;
    
    return 10-intentos;
}

int verificar_nivel_personalizado(unsigned int nivel_adivinado, unsigned int nivel_pokemon){
    return (int)(nivel_adivinado - nivel_pokemon);
}

const char* verificacion_a_string_personalizado(int resultado_verificacion){
    if(resultado_verificacion == 0){
        return "Bien";
    }else{
        return "Mal";
    }

}

void dadoUnSimulador_puedoAgregarDificultad(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    dificultad.nombre = "Facil";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;

    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ExitoSimulacion), "Se pudo agregar la dificultad");
   

    simulador_destruir(simulador);
}

//----------------------------------------------------------------------------------------------//

int main(){

    pa2m_nuevo_grupo("Pruebas de creacion y destruccion");
    dadoUnHospital_puedoCrearYDestruirUnSimulador();
    dadoUnHospitalNULL_noPuedoCrearUnSimulador();

    pa2m_nuevo_grupo("Pruebas con simulador NULL");
    dadoUnSimuladorNULL_noPuedoHacerOperaciones();


    pa2m_nuevo_grupo("Pruebas de evento OBTENER");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoObtenerEstadisticas();
    dadoUnSimuladorSinEntrenadoresAtendidos_sinEstructuraDeEstadisticas_noPuedoObtenerEstadisticas();


    pa2m_nuevo_grupo("Pruebas de evento ATENDER PROXIMO ENTRENADOR");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderAlProximoEntrenador();
    dadoUnSimuladorSinEntrenadoresAtendidos_ySinEntrenadoresEnEspera_noPuedoAtenderAlProximoEntrenador();
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderVariosEntrenadores();


    pa2m_nuevo_grupo("Pruebas de evento OBTENER INFORMACION POKEMON");
    dadoUnSimuladorSinEntrenadoresAtendidos_noPuedoObtenerLaInformacionDelPokemonEnTratamiento();
    dadoUnSimuladorConUnEntrenadorAtendido_puedoObtenerLaInformacionDelPokemonEnTratamiento();

    pa2m_nuevo_grupo("Pruebas de evento AGREGAR DIFICULTAD");
    dadoUnSimulador_puedoAgregarDificultad();



    return pa2m_mostrar_reporte();
}
