#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <string.h>

/* Funciones auxiliares */

/*
 * Devuelve el puntaje respectivo a la dificultad facil
 */
unsigned int puntaje_personalizado(const unsigned int intentos){
    if(intentos > 10)
        return 0;
    
    return 11-intentos;
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
    pa2m_afirmar((estadisticas.pokemon_en_espera == 0), "La cantidad de pokemon en espera es igual a 0");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");


    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_conUnHospitalDeUnEntrenador_puedoObtenerEstadisticas(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;
    
    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 1), "La cantidad de eventos simulados es 1");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 0), "La cantidad de entrenadores atendidos es 0");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas.pokemon_en_espera == 0), "La cantidad de pokemon en espera es 0");
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
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas.pokemon_en_espera == 3), "La cantidad de pokemon en espera es igual a 3");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_y_SinEntrenadoresEnEspera_noPuedoAtenderAlProximoEntrenador(){
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
    pa2m_afirmar((estadisticas.pokemon_en_espera == 0), "La cantidad de pokemon en espera es igual a 0");
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
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 13");
    pa2m_afirmar((estadisticas.pokemon_en_espera == 12), "La cantidad de pokemon en espera es igual a 12");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 0), "La cantidad de puntos es 0");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderATodosLosEntrenadores(){
        hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    EstadisticasSimulacion estadisticas;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    size_t i = 0;
    while (i < estadisticas.entrenadores_totales){
        simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
        i++;
    }
    

    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);

    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == estadisticas.entrenadores_totales+2), "La cantidad de eventos simulados es la cantidad de entrenadores mas los 2 llamados de estadisticas");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == estadisticas.entrenadores_totales), "La cantidad de entrenadores atendidos es la cantidad total");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 0), "La cantidad de pokemon atendidos es 0");
    pa2m_afirmar((estadisticas.pokemon_en_espera == hospital_cantidad_pokemon(h)-1), "La cantidad de pokemon en espera es igual a la cantidad total del hospital menos 1");
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

void dadoUnSimuladorConEntrenadoresAtendidos_noPuedoObtenerLaInformacionDelPokemonEnTratamiento_siInformacionEsNULL(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionPokemon* informacion = NULL;

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, informacion) == ErrorSimulacion), "No puedo obtener la informacion del pokemon en tratamiento ya que informacion es NULL");

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

void dadoUnSimuladorSinEntrenadoresAtendidos_noPuedoAdivinarElNivelDePokemon(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    Intento intento;
    intento.nivel_adivinado = 34;

    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ErrorSimulacion), "No puedo intentar adivinar el nivel ya que no hay entrenadores atendidos");

    simulador_destruir(simulador);
}

void dadoUnSimuladorConEntrenadoresAtendidos_noPuedoAdivinarElNivelDePokemon_siLePasoIntentoNULL(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    Intento* intento = NULL;

    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, intento) == ErrorSimulacion), "No puedo intentar adivinar el nivel ya que el intento es NULL");

    simulador_destruir(simulador);
}

void dadoUnSimuladorConEntrenadoresAtendidos_puedoAdivinarElNivelDePokemon(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    Intento intento;

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    intento.nivel_adivinado = 34;
    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Puedo intentar adivinar el nivel");
    pa2m_afirmar((intento.es_correcto == false), "El nivel era incorrecto");
    pa2m_afirmar((strcmp(intento.resultado_string, "Te sobran menos de 25 niveles") == 0), "La frase de resultado es la esperada");

    intento.nivel_adivinado = 10;
    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Puedo intentar adivinar el nivel");
    pa2m_afirmar((intento.es_correcto == true), "El nivel era correcto");
    pa2m_afirmar((strcmp(intento.resultado_string, "Acertaste el nivel del pokemon!") == 0), "La frase de resultado es la esperada");

    simulador_destruir(simulador);
}

void dadoUnSimuladorConEntrenadoresAtendidos_y_unaDificultadAgregada_puedoAdivinarElNivelDePokemon(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    dificultad.nombre = "Personalizada";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;
    int id = 3;
    Intento intento;
    EstadisticasSimulacion estadisticas;

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    simulador_simular_evento(simulador, AgregarDificultad, &dificultad);
    simulador_simular_evento(simulador, SeleccionarDificultad, &id);

    intento.nivel_adivinado = 14;
    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Puedo intentar adivinar el pokemon");
    pa2m_afirmar((intento.es_correcto == false), "El numero es incorrecto");
    pa2m_afirmar((strcmp(intento.resultado_string, "Mal") == 0), "El string resultado es correcto");

    intento.nivel_adivinado = 10;
    pa2m_afirmar((simulador_simular_evento(simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion), "Puedo intentar adivinar el pokemon");
    pa2m_afirmar((intento.es_correcto == true), "El numero es correcto");
    pa2m_afirmar((strcmp(intento.resultado_string, "Bien") == 0), "El string resultado es correcto");

    simulador_simular_evento(simulador, ObtenerEstadisticas, &estadisticas);
    pa2m_afirmar((estadisticas.cantidad_eventos_simulados == 6), "La cantidad de eventos simulados es 6");
    pa2m_afirmar((estadisticas.entrenadores_atendidos == 1), "La cantidad de entrenadores atendidos es 1");
    pa2m_afirmar((estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(h)), "La cantidad de entrenadores totales es igual a la del hospital");
    pa2m_afirmar((estadisticas.pokemon_atendidos == 1), "La cantidad de pokemon atendidos es la cantidad de pokemon del entrenador");
    pa2m_afirmar((estadisticas.pokemon_en_espera == 2), "La cantidad de pokemon en espera es igual a 2");
    pa2m_afirmar((estadisticas.pokemon_totales == hospital_cantidad_pokemon(h)), "La cantidad total de pokemon es igual a la cantidad total del hospital");
    pa2m_afirmar((estadisticas.puntos == 9), "La cantidad de puntos es 9");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoAgregarDificultad(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    dificultad.nombre = "Personalizada";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;

    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ExitoSimulacion), "Se pudo agregar la dificultad");

    simulador_destruir(simulador);
}

void dadoUnSimulador_noPuedoAgregarDificultadConNombreRepetido(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    dificultad.nombre = "Normal";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;

    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion), "No se pudo agregar la dificultad de nombre repetido");

    simulador_destruir(simulador);
}

void dadoUnSimulador_noPuedoAgregarDificultadConCampoNULL(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    dificultad.nombre = NULL;
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;

    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion), "No se pudo agregar la dificultad de nombre NULL");

    dificultad.nombre = "Personalizada";
    dificultad.calcular_puntaje = NULL;;
    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion), "No se pudo agregar la dificultad de calcular puntaje NULL");

    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = NULL;
    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion), "No se pudo agregar la dificultad de verificar nivel NULL");

    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = NULL;
    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion), "No se pudo agregar la dificultad de verificacion a string NULL");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoSeleccionarDificultad_y_VerificoQueEsteEnUso(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    int id = 1;
    InformacionDificultad info;

    pa2m_afirmar((simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion), "Puedo seleccionar la dificultad con id 1");
    
    info.id = 1;
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Puedo obtener la informacion de la dificultad");
    pa2m_afirmar((info.en_uso == true), "La dificultad esta en uso");

    info.id = 0;
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    pa2m_afirmar((info.en_uso == false), "La dificultad que antes estaba en uso, ya no lo esta");

    simulador_destruir(simulador);
}

void dadoUnSimulador_conUnaDificultadAgregada_laSelecciono_y_VerificoQueEsteEnUso(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionDificultad info;
    DatosDificultad dificultad;
    dificultad.nombre = "Personalizada";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;
    int id = 3;

    simulador_simular_evento(simulador, AgregarDificultad, &dificultad);

    pa2m_afirmar((simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion), "Puedo seleccionar la dificultad con id 3");
    
    info.id = 3;
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Puedo obtener la informacion de la dificultad");
    pa2m_afirmar((info.en_uso == true), "La dificultad esta en uso");

    info.id = 0;
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    pa2m_afirmar((info.en_uso == false), "La dificultad que antes estaba en uso, ya no lo esta");

    simulador_destruir(simulador);
}

void dadoUnSimulador_seleccionarDificultadConIdInvalidoDevuelveError(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    int id = 3;
    InformacionDificultad info;

    pa2m_afirmar((simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ErrorSimulacion), "No puedo seleccionar la dificultad con id 3 (no existe)");
    
    info.id = 0;
    simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info);
    pa2m_afirmar((info.en_uso == true), "La dificultad que antes estaba en uso, sigue en uso");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoObtenerSusDificultades(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionDificultad info;
    info.id = 0;
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Se pudo obtener la dificultad Facil");
    pa2m_afirmar((info.id == 0), "El id es correcto");
    pa2m_afirmar((strcmp(info.nombre_dificultad, "Facil") == 0), "El nombre es correcto");
    pa2m_afirmar((info.en_uso == true), "Esta en uso");

    info.id = 1;
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Se pudo obtener la dificultad Normal");
    pa2m_afirmar((info.id == 1), "El id es correcto");
    pa2m_afirmar((strcmp(info.nombre_dificultad, "Normal") == 0), "El nombre es correcto");
    pa2m_afirmar((info.en_uso == false), "No esta en uso");

    info.id = 2;
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Se pudo obtener la dificultad Dificil");
    pa2m_afirmar((info.id == 2), "El id es correcto");
    pa2m_afirmar((strcmp(info.nombre_dificultad, "Dificil") == 0), "El nombre es correcto");
    pa2m_afirmar((info.en_uso == false), "No esta en uso");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoAgregarDificultad_y_LuegoBuscarla(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad dificultad;
    InformacionDificultad info;
    dificultad.nombre = "Prueba";
    dificultad.calcular_puntaje = puntaje_personalizado;
    dificultad.verificar_nivel = verificar_nivel_personalizado;
    dificultad.verificacion_a_string = verificacion_a_string_personalizado;

    info.id = 3;
    simulador_simular_evento(simulador, AgregarDificultad, &dificultad);
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Se pudo obtener la dificultad personalizada");
    pa2m_afirmar((info.id == 3), "El id es correcto");
    pa2m_afirmar((strcmp(info.nombre_dificultad, "Prueba") == 0), "El nombre es correcto");
    pa2m_afirmar((info.en_uso == false), "No esta en uso");

    simulador_destruir(simulador);
}

void dadoUnSimulador_noPuedoObtenerUnaDificultadConIdInvalido(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    InformacionDificultad info;

    info.id = 3;

    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ErrorSimulacion), "No se pudo obtener la dificultad con id invalido");
    pa2m_afirmar((info.id == -1), "El id cambio a -1");
    pa2m_afirmar((info.nombre_dificultad == NULL), "El nombre es NULL");
    pa2m_afirmar((info.en_uso == false), "No esta en uso");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoFinalizarSimulacion(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);

    pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Puedo finalizar la simulacion");
    simulador_destruir(simulador);
}

void dadoUnSimuladorConLaSimulacionFinalizada_noPuedoSimularEventos(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);

    pa2m_afirmar((simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion), "Puedo finalizar la simulacion");
    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion), "No puedo realizar otro evento con la simulacion finalizada");
    simulador_destruir(simulador);
}
/*
void xd(){
    hospital_t* h = hospital_crear();
    hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital");
    simulador_t* simulador = simulador_crear(h);
    DatosDificultad* dificultad = malloc(sizeof(DatosDificultad));
    InformacionDificultad info;
    char nombre[] = "Prueba";
    dificultad->nombre = nombre;
    dificultad->calcular_puntaje = puntaje_personalizado;
    dificultad->verificar_nivel = verificar_nivel_personalizado;
    dificultad->verificacion_a_string = verificacion_a_string_personalizado;
    int id = 3;
    info.id = 3;
    
    pa2m_afirmar((simulador_simular_evento(simulador, AgregarDificultad, dificultad) == ExitoSimulacion), "Se puede agregar la dificultad");

    strcpy(nombre, "aaa");

    pa2m_afirmar((simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion), "Se puede atender al proximo entrenador");
    pa2m_afirmar((simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion), "Se puede seleccionar la dificultad");
    pa2m_afirmar((simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info) == ExitoSimulacion), "Se pudo obtener la dificultad personalizada");
    pa2m_afirmar((info.en_uso == true), "Esta en uso");
    pa2m_afirmar((strcmp(info.nombre_dificultad, "Prueba") == 0), "El nombre es correcto");

    free(dificultad);
    simulador_destruir(simulador);
} */

//----------------------------------------------------------------------------------------------//

int main(){

    pa2m_nuevo_grupo("Pruebas de creacion y destruccion");
    dadoUnHospital_puedoCrearYDestruirUnSimulador();
    dadoUnHospitalNULL_noPuedoCrearUnSimulador();


    pa2m_nuevo_grupo("Pruebas con simulador NULL");
    dadoUnSimuladorNULL_noPuedoHacerOperaciones();


    pa2m_nuevo_grupo("Pruebas de evento OBTENER ESTADISTICAS");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoObtenerEstadisticas();
    dadoUnSimuladorSinEntrenadoresAtendidos_conUnHospitalDeUnEntrenador_puedoObtenerEstadisticas();
    dadoUnSimuladorSinEntrenadoresAtendidos_sinEstructuraDeEstadisticas_noPuedoObtenerEstadisticas();


    pa2m_nuevo_grupo("Pruebas de evento ATENDER PROXIMO ENTRENADOR");
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderAlProximoEntrenador();
    dadoUnSimuladorSinEntrenadoresAtendidos_y_SinEntrenadoresEnEspera_noPuedoAtenderAlProximoEntrenador();
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderVariosEntrenadores();
    dadoUnSimuladorSinEntrenadoresAtendidos_puedoAtenderATodosLosEntrenadores();


    pa2m_nuevo_grupo("Pruebas de evento OBTENER INFORMACION POKEMON");
    dadoUnSimuladorSinEntrenadoresAtendidos_noPuedoObtenerLaInformacionDelPokemonEnTratamiento();
    dadoUnSimuladorConEntrenadoresAtendidos_noPuedoObtenerLaInformacionDelPokemonEnTratamiento_siInformacionEsNULL();
    dadoUnSimuladorConUnEntrenadorAtendido_puedoObtenerLaInformacionDelPokemonEnTratamiento();


    pa2m_nuevo_grupo("Pruebas de evento ADIVINAR POKEMON");
    dadoUnSimuladorSinEntrenadoresAtendidos_noPuedoAdivinarElNivelDePokemon();
    dadoUnSimuladorConEntrenadoresAtendidos_noPuedoAdivinarElNivelDePokemon_siLePasoIntentoNULL();
    dadoUnSimuladorConEntrenadoresAtendidos_puedoAdivinarElNivelDePokemon();
    dadoUnSimuladorConEntrenadoresAtendidos_y_unaDificultadAgregada_puedoAdivinarElNivelDePokemon();


    pa2m_nuevo_grupo("Pruebas de evento AGREGAR DIFICULTAD");
    dadoUnSimulador_puedoAgregarDificultad();
    dadoUnSimulador_noPuedoAgregarDificultadConNombreRepetido();
    dadoUnSimulador_noPuedoAgregarDificultadConCampoNULL();


    pa2m_nuevo_grupo("Pruebas de evento SELECCIONAR DIFICULTAD");
    dadoUnSimulador_puedoSeleccionarDificultad_y_VerificoQueEsteEnUso();
    dadoUnSimulador_conUnaDificultadAgregada_laSelecciono_y_VerificoQueEsteEnUso();
    dadoUnSimulador_seleccionarDificultadConIdInvalidoDevuelveError();


    pa2m_nuevo_grupo("Pruebas de evento OBTENER DIFICULTAD");
    dadoUnSimulador_puedoObtenerSusDificultades();
    dadoUnSimulador_puedoAgregarDificultad_y_LuegoBuscarla();
    dadoUnSimulador_noPuedoObtenerUnaDificultadConIdInvalido();


    pa2m_nuevo_grupo("Pruebas de evento FINALIZAR SIMULACION");
    dadoUnSimulador_puedoFinalizarSimulacion();
    dadoUnSimuladorConLaSimulacionFinalizada_noPuedoSimularEventos();

/*
    pa2m_nuevo_grupo("Pruebas de evento xd");
    xd();*/

    return pa2m_mostrar_reporte();
}
