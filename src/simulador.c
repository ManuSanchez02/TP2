#include "simulador.h"
#include "hospital_interno.h"
#include "heap.h"
#include "cola.h"
#include <string.h>

const size_t TAMANIO_INICIAL_HEAP = 6; //! Explicar constante
const int RESULTADO_CORRECTO = 0;
const int ID_DIFICULTAD_NO_ENCONTRADA = -1;

const unsigned int PUNTAJE_MAXIMO_FACIL = 10;
const unsigned int PUNTAJE_MAXIMO_NORMAL = 50;
const unsigned int PUNTAJE_MAXIMO_DIFICIL = 200;

const unsigned int MULTIPLICADOR_NORMAL = 2;

struct _simulador_t{
    hospital_t* hospital;
    cola_t* entrenadores_en_espera;
    heap_t* pokemon_atendidos;          // SACRIFICO TAMAÑO CON TAL DE OPTIMIZAR LOS TIEMPOS --> NO RECORRER LA LISTA DE HOSPITAL TODO EL TIEMPO
    pokemon_t* pokemon_en_tratamiento;
    lista_t* dificultades;
    DatosDificultad* dificultad_seleccionada; //! Explicar por que uso puntero en vez de id -> es mas simple el acceso (no hay que iterar la lista)
    size_t cantidad_intentos;
    unsigned puntos;
    unsigned cantidad_eventos_simulados;
    unsigned cantidad_pokemon_atendidos;
    bool activo;
};

typedef struct datos_atender_pokemon_entrenador{
    heap_t* pokemon_atendidos;
    entrenador_t* entrenador;
    unsigned* cantidad_pokemon_atendidos;
}datos_atender_pokemon_entrenador_t;

typedef struct puntero_y_retorno{ //! EXPLICAR XQ ESTO -> Es la unica forma de saber si se encontro o no
    void* puntero;
    bool retorno;
} puntero_y_retorno_t;

//-----------------------------------------------------//
/*                FUNCIONES AUXILIARES                 */
//-----------------------------------------------------//


bool dificultad_destruir(void* _dificultad, void* aux){
    if(!_dificultad)
        return false;

    DatosDificultad* dificultad = _dificultad;

    free(dificultad);
    return true;
}

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
        return heap_insertar(datos_aux->pokemon_atendidos, pokemon_a_atender) != NULL;
    }


    return true;
}

/*
 * Pre: entrenador_a_encolar debe ser un puntero a entrenador, datos_aux un puntero a un puntero_y_retorno_t con un puntero a una cola de entrenadores
 * Post: Devuelve true si pudo encolar entrenador_a_encolar en cola_entrenadores y false en caso contrario.
 */
bool encolar_entrenadores(void* entrenador_a_encolar, void* _datos_aux){
    if(!entrenador_a_encolar || !_datos_aux)
        return false;

    puntero_y_retorno_t* datos_aux = _datos_aux;

    datos_aux->retorno = cola_encolar(datos_aux->puntero, entrenador_a_encolar) != NULL;
    return datos_aux->retorno;
}

/*
 * Pre: _dificultad debe ser puntero a DatosDificultad y _datos_aux un puntero a puntero_y_retorno_t con un puntero a la dificultad a verificar
 * Post: Devuelve true si las dificultades tienen nombre distinto y false en caso contrario. Guarda en datos_aux->retorno el valor de retorno de la funcion
 */
bool dificultad_no_existe(void* _dificultad, void* _datos_aux){
    if(!_dificultad || !_datos_aux)
        return false;

    DatosDificultad* dificultad= _dificultad;
    puntero_y_retorno_t* datos_aux = _datos_aux;

    datos_aux->retorno = strcmp(((DatosDificultad*)(datos_aux->puntero))->nombre, dificultad->nombre) != 0;

    return datos_aux->retorno;
}

int potencia(int base, unsigned int exponente){
    int resultado = 1;

    for(size_t i = 0; i < exponente; i++)
        resultado *= base;

    return resultado;
}

bool campos_validos(DatosDificultad* dificultad){
    if(!dificultad)
        return false;

    return (dificultad->calcular_puntaje && dificultad->nombre && dificultad->verificacion_a_string && dificultad->verificar_nivel);
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
    estadisticas->pokemon_en_espera = (unsigned) heap_tamanio(simulador->pokemon_atendidos);
    estadisticas->puntos = simulador->puntos;
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;

    return ExitoSimulacion;
}


/*
 * Pre: Debe haber un entrenador en cola
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

/*
 * Pre: Debe haber un pokemon en tratamiento
 * Post: Si no hay pokemon en tratamiento, llena los campos de info_pokemon con NULL
 *       Si el puntero a simulador o a info_pokemon es NULL, devuelve ErrorSimulacion.
 *       En otro caso, llena los campos de info_pokemon con el puntero al nombre del pokemon en tratamiento y el nombre del entrenador del mismo, y devuelve ExitoSimulacion
 */
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

/*
 * Pre: Debe haber una dificultad seleccionada y un pokemon en tratamiento
 * Post: Carga intento con los datos correspondientes dependiendo de simulador->dificultad. Devuelve ExitoSimulacion si tuvo exito y ErrorSimulacion en caso contrario
 */
ResultadoSimulacion adivinar_nivel_pokemon(simulador_t* simulador, Intento* intento){
    if(!simulador || !intento || !simulador->pokemon_en_tratamiento || !simulador->dificultad_seleccionada)
        return ErrorSimulacion;

    simulador->cantidad_intentos++;
    int resultado_verificar_nivel = simulador->dificultad_seleccionada->verificar_nivel(intento->nivel_adivinado, (unsigned)pokemon_nivel(simulador->pokemon_en_tratamiento));  
    
    intento->es_correcto = resultado_verificar_nivel == RESULTADO_CORRECTO;
    if(intento->es_correcto){
        simulador->cantidad_pokemon_atendidos++;
        simulador->pokemon_en_tratamiento = heap_extraer_raiz(simulador->pokemon_atendidos);
        simulador->puntos += simulador->dificultad_seleccionada->calcular_puntaje((unsigned int) simulador->cantidad_intentos);
        simulador->cantidad_intentos = 0;
    }

    intento->resultado_string = simulador->dificultad_seleccionada->verificacion_a_string(resultado_verificar_nivel);
    return ExitoSimulacion;
}

/*
 * Pre: el id debe ser un puntero a int. Este int debe ser un id valido de dificultad
 * Post: Devuelve ExitoSimulacion si pudo seleccionar la dificultad y ErrorSimulacion en caso contrario.
 */
ResultadoSimulacion seleccionar_dificultad(simulador_t* simulador, int* id){
    if(!simulador || !id)
        return ErrorSimulacion;

    DatosDificultad* dificultad_seleccionada = lista_elemento_en_posicion(simulador->dificultades, (size_t)*id);

    if(!dificultad_seleccionada)
        return ErrorSimulacion;

    simulador->dificultad_seleccionada = dificultad_seleccionada;
    return ExitoSimulacion;
}

/*
 * Pre: 
 * Post: Inserta la dificultad en la lista de dificultades y devuelve ExitoSimulacion si fue exitoso, y ErrorSimulacion en caso contrario
 *       Si no hay dificultad seleccionada, selecciona la agregada
 */
ResultadoSimulacion agregar_dificultad(simulador_t* simulador, DatosDificultad* dificultad){
    if(!simulador || !campos_validos(dificultad))
        return ErrorSimulacion;

    puntero_y_retorno_t datos_aux;
    datos_aux.puntero = dificultad;
    datos_aux.retorno = true;

    lista_con_cada_elemento(simulador->dificultades, dificultad_no_existe, &datos_aux);
    DatosDificultad* dificultad_copia = malloc(sizeof(DatosDificultad));
    if(!dificultad_copia ||  !datos_aux.retorno){
        dificultad_destruir(dificultad_copia, NULL);
        return ErrorSimulacion;
    }

    dificultad_copia->nombre = dificultad->nombre;
    dificultad_copia->calcular_puntaje = dificultad->calcular_puntaje;
    dificultad_copia->verificacion_a_string = dificultad->verificacion_a_string;
    dificultad_copia->verificar_nivel = dificultad->verificar_nivel;
    
    if(!lista_insertar(simulador->dificultades, dificultad_copia))
        return ErrorSimulacion;

    if(!simulador->dificultad_seleccionada){
        int id = 0;
        seleccionar_dificultad(simulador, &id);
    }

    return ExitoSimulacion;
}


ResultadoSimulacion obtener_informacion_dificultad(simulador_t* simulador, InformacionDificultad* info_dificultad){
    if(!simulador || !info_dificultad)
        return ErrorSimulacion;

    DatosDificultad* dificultad_buscada = lista_elemento_en_posicion(simulador->dificultades, (size_t)info_dificultad->id);
    if(!dificultad_buscada){
        info_dificultad->id = ID_DIFICULTAD_NO_ENCONTRADA;
        info_dificultad->en_uso = false;
        info_dificultad->nombre_dificultad = NULL;
        return ErrorSimulacion;
    }
    
    info_dificultad->en_uso = simulador->dificultad_seleccionada == dificultad_buscada;
    info_dificultad->nombre_dificultad = dificultad_buscada->nombre;

    return ExitoSimulacion;
}


ResultadoSimulacion finalizar_simulacion(simulador_t* simulador){
    if(!simulador)
        return ErrorSimulacion;

    simulador->activo = false;

    return ExitoSimulacion; 
}

//-----------------------------------------------------//
/*            FUNCIONES DE DIFICULTADES                */
//-----------------------------------------------------//

int verificar_nivel(unsigned int nivel_adivinado, unsigned int nivel_pokemon){
    return (int)(nivel_adivinado - nivel_pokemon);
}

unsigned int puntaje_facil(const unsigned int intentos){
    if(intentos > PUNTAJE_MAXIMO_FACIL)
        return 0;
    
    return PUNTAJE_MAXIMO_FACIL-intentos;
}

unsigned int puntaje_normal(const unsigned int intentos){
    if(intentos*MULTIPLICADOR_NORMAL > PUNTAJE_MAXIMO_NORMAL)
        return 0;
    
    return PUNTAJE_MAXIMO_NORMAL-MULTIPLICADOR_NORMAL*intentos;
}

unsigned int puntaje_dificil(const unsigned int intentos){
    return PUNTAJE_MAXIMO_DIFICIL/(unsigned int)potencia(2, intentos);
}


const char* verificacion_a_string_facil(int resultado_verificacion){
    int distancia = abs(resultado_verificacion);
    if(distancia == 0){
        return "Acertaste el nivel del pokemon!";
    }else if(distancia < 3){
        return resultado_verificacion > 0 ? "Te sobran menos de 3 niveles!" : "Te faltan menos de 3 niveles!";
    }else if(distancia < 5){
        return resultado_verificacion > 0 ? "Te sobran menos de 5 niveles!" : "Te faltan menos de 5 niveles!";
    }else if(distancia < 10){
        return resultado_verificacion > 0 ? "Te sobran menos de 10 niveles" : "Te faltan menos de 10 niveles";
    }else if(distancia < 25){
        return resultado_verificacion > 0 ? "Te sobran menos de 25 niveles" : "Te faltan menos de 25 niveles";
    }else if(distancia < 50){
        return resultado_verificacion > 0 ? "Te sobran menos de 50 niveles" : "Te faltan menos de 50 niveles";
    }else if(distancia < 80){
        return resultado_verificacion > 0 ? "Te sobran menos de 80 niveles" : "Te faltan menos de 80 niveles";
    }else{
        return resultado_verificacion > 0 ? "Te sobran 80 niveles o mas :(" : "Te faltan 80 niveles o mas :(";
    }
}

const char* verificacion_a_string_normal(int resultado_verificacion){
    resultado_verificacion = abs(resultado_verificacion);
    if(resultado_verificacion == 0){
        return "Acertaste el nivel del pokemon!";
    }else if(resultado_verificacion < 3){
        return "Estas a menos de 3 niveles de distancia!";
    }else if(resultado_verificacion < 5){
        return "Estas a menos de 5 niveles de distancia!";
    }else if(resultado_verificacion < 10){
        return "Estas a menos de 10 niveles de distancia";
    }else if(resultado_verificacion < 25){
        return "Estas a menos de 25 niveles de distancia";
    }else if(resultado_verificacion < 50){
        return "Estas a menos de 50 niveles de distancia";
    }else if(resultado_verificacion < 80){
        return "Estas a menos de 80 niveles de distancia";
    }else{
        return "Estas a mas de 80 niveles de distancia";
    }
}

const char* verificacion_a_string_dificil(int resultado_verificacion){
    resultado_verificacion = abs(resultado_verificacion);
    if(resultado_verificacion == 0){
        return "Acertaste el nivel del pokemon!";
    }else if(resultado_verificacion < 5){
        return "Estas muy cerca!";
    }else if(resultado_verificacion < 10){
        return "Estas cerca";
    }else if(resultado_verificacion < 25){
        return "No estas ni lejos ni cerca";
    }else if(resultado_verificacion < 50){
        return "Estas lejos";
    }else{
        return "Ufff le pifiaste mal";
    }
}

bool agregar_dificultades_iniciales(simulador_t* simulador){
    if(!simulador)
        return false;

    DatosDificultad facil;
    DatosDificultad normal;
    DatosDificultad dificil;

    facil.nombre = "Facil";
    facil.calcular_puntaje = puntaje_facil;
    facil.verificar_nivel = verificar_nivel;
    facil.verificacion_a_string = verificacion_a_string_facil;
    agregar_dificultad(simulador, &facil);

    normal.nombre = "Normal";
    normal.calcular_puntaje = puntaje_normal;
    normal.verificar_nivel = verificar_nivel;
    normal.verificacion_a_string = verificacion_a_string_normal;
    agregar_dificultad(simulador, &normal);


    dificil.nombre = "Dificil";
    dificil.calcular_puntaje = puntaje_dificil;
    dificil.verificar_nivel = verificar_nivel; 
    dificil.verificacion_a_string = verificacion_a_string_dificil;
    agregar_dificultad(simulador, &dificil);


    return true;
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
    simulador->dificultades = lista_crear();
    if(!simulador->entrenadores_en_espera || !simulador->pokemon_atendidos || !simulador->dificultades){
        heap_destruir(simulador->pokemon_atendidos);
        cola_destruir(simulador->entrenadores_en_espera);
        lista_destruir(simulador->dificultades);
        free(simulador);
        return NULL;
    }

    simulador->pokemon_en_tratamiento = NULL;
    simulador->dificultad_seleccionada = NULL;
    simulador->cantidad_eventos_simulados = 0;
    simulador->cantidad_pokemon_atendidos = 0;
    simulador->puntos = 0;
    simulador->cantidad_intentos = 0;

    puntero_y_retorno_t datos_aux;
    datos_aux.puntero = simulador->entrenadores_en_espera;
    datos_aux.retorno = true;

    lista_con_cada_elemento(simulador->hospital->lista_entrenadores, encolar_entrenadores, &datos_aux);
    if(!datos_aux.retorno || !agregar_dificultades_iniciales(simulador)){
        simulador_destruir(simulador);
        return NULL;
    }

    simulador->activo = true;

    return simulador;
}


ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
    if(!simulador || !simulador->activo)
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
            resultado = adivinar_nivel_pokemon(simulador, datos);
            break;

        case AgregarDificultad:
            resultado = agregar_dificultad(simulador, datos);
            break;

        case SeleccionarDificultad:
            resultado = seleccionar_dificultad(simulador, datos);
            break;

        case ObtenerInformacionDificultad:
            resultado = obtener_informacion_dificultad(simulador, datos);
            break;

        case FinalizarSimulacion:
            resultado = finalizar_simulacion(simulador);
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
    lista_con_cada_elemento(simulador->dificultades, dificultad_destruir, NULL);
    lista_destruir(simulador->dificultades);
    hospital_destruir(simulador->hospital);
    free(simulador);
}
