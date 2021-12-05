#ifndef HOSPITAL_INTERNO_H_
#define HOSPITAL_INTERNO_H_

#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "split.h"


typedef struct _entrenador_pkm_t{
    char* nombre;
    int id;
} entrenador_t;

struct _hospital_pkm_t{
    size_t cantidad_actual_pokemon;
    size_t cantidad_maxima_pokemon;
    pokemon_t* vector_pokemon;
    size_t cantidad_actual_entrenadores;
    size_t cantidad_maxima_entrenadores;
    entrenador_t* vector_entrenadores;
};

struct _pkm_t{
    char* nombre;
    size_t nivel;
    int id_entrenador;
};


/*
 * Pre: archivo debe estar abierto
 * Post: - devuelve un puntero a una direccion de memoria dinamica que contiene la siguiente linea del archivo.
 *       - avanza el puntero de lectura/escritura al comienzo de la siguiente linea.
 */
char* leer_linea(FILE* archivo);

/*
 * Pre: el tope del vector debe estar inicializado y estar acorde al vector.
 * Post: si el vector no esta inicializado, no pasa nada. Si lo esta, lo ordena alfabeticamente 
 */
void ordenar_alfabeticamente(pokemon_t* vector, size_t tope);

/*
 * Pre: la cantidad_elementos debe estar inicializado y estar acorde al vector.
 * Post: Si el vector es NULL, crea un vector de tamaño sizeof(entrenador_t), le agrega elemento y aumenta cantidad_elementos.
 *       Si ya estaba creado, agrega elemento en la ultima posicion y aumenta la cantidad_elementos.
 */
entrenador_t* agregar_entrenador(entrenador_t* vector, entrenador_t elemento, size_t* cantidad_elementos, size_t* cantidad_maxima);

/*
 * Pre: la cantidad_elementos debe estar inicializado y estar acorde al vector.
 * Post: Si el vector es NULL, crea un vector de tamaño sizeof(pokemon_t), le agrega elemento y aumenta cantidad_elementos.
 *       Si ya estaba creado, agrega elemento en la ultima posicion y aumenta la cantidad_elementos.
 */
pokemon_t* agregar_pokemon(pokemon_t* vector, pokemon_t elemento, size_t* cantidad_elementos, size_t* cantidad_maxima);

/*
 * Pre: nombre debe ser distinto de NULL
 * Post: Devuelve TRUE si se pudo inicializar al entrenador y FALSE en caso contrario
 */
bool inicializar_entrenador(entrenador_t* entrenador, int id, char* nombre);

/*
 * Pre: nombre debe ser distinto de NULL
 * Post: Devuelve TRUE si se pudo inicializar al pokemon y FALSE en caso contrario
 */
bool inicializar_pokemon(pokemon_t* pokemon, char* nombre, size_t nivel);

/*
 * Pre: -
 * Post: Parsea los elementos de elementos_linea y los agrega a los respectivos campos de hospital_t.
 *       Devuelve true si el proceso fue un exito y false en caso contrario
 *       Libera la memoria asignada a elementos_linea
 */
bool parsear_linea(hospital_t* hospital, char** elementos_linea);


/*
 * Pre: -
 * Post: Devuelve true si el proceso fue un exito y false en caso contrario
 */
bool hospital_copiar(hospital_t* hospital_copia, hospital_t* hospital_original);

#endif // HOSPITAL_INTERNO_H_