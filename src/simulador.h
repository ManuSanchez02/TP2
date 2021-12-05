#ifndef SIMULADOR_H_
#define SIMULADOR_H_

#include "hospital.h"

typedef struct _simulador_t simulador_t;

typedef enum {
    ObtenerEstadisticas,

    AtenderProximoEntrenador,

    ObtenerInformacionPokemonEnTratamiento,
    AdivinarNivelPokemon,

    AgregarDificultad,
    SeleccionarDificultad,
    ObtenerInformacionDificultad,

    FinalizarSimulacion
} EventoSimulacion;

typedef enum {
    ErrorSimulacion,
    ExitoSimulacion
} ResultadoSimulacion;

typedef struct{
    unsigned entrenadores_atendidos;
    unsigned entrenadores_totales;
    unsigned pokemon_atendidos;
    unsigned pokemon_en_espera;
    unsigned pokemon_totales;
    unsigned puntos;
    unsigned cantidad_eventos_simulados;
}EstadisticasSimulacion;

typedef struct{
    const char* nombre_pokemon;
    const char* nombre_entrenador;
}InformacionPokemon;

typedef struct{
    unsigned nivel_adivinado;
    bool es_correcto;
    const char* resultado_string;
}Intento;

typedef struct{
    const char* nombre;
    unsigned (*calcular_puntaje)(unsigned cantidad_intentos);
    int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon);
    const char* (*verificacion_a_string)(int resultado_verificacion);
}DatosDificultad;

typedef struct{
    const char* nombre_dificultad;
    bool en_uso;
    int id;
}InformacionDificultad;

/**
 * Crea un simulador para un hospital. El simulador toma control del hospital y
 * el mismo no debe ser modificado ni liberado por fuera del simulador.
 *
 * Devuelve el simulador creado o NULL en caso de error.
 */
simulador_t* simulador_crear(hospital_t* hospital);

/**
 * Simula un evento. Cada evento puede recibir un puntero a un dato que depende
 * de cada evento. En caso de no necesitarse, se debe pasar NULL.
 *
 * Ejecuta el evento correspondiente (ver la documentación) y en caso de ser
 * necesario modifica la información apuntada por dato.
 *
 * Devuelve ErrorSimulacion si no existe el simulador o existe algún error en el
 * manejo de memoria.
 *
 * Devuelve ExitoSimulacion o ErrorSimulacion según corresponda a cada evento.
 */
ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos);

/**
 * Destruye el simulador y libera la memoria asociada (incluida la del hospital).
 */
void simulador_destruir(simulador_t* simulador);

#endif // SIMULADOR_H_
