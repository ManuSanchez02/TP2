#ifndef HOSPITAL_INTERNO_H_
#define HOSPITAL_INTERNO_H_

#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "split.h"
#include "lista.h"
#include "cola.h"


struct _entrenador_t{
    char* nombre;
    int id;
};

struct _hospital_pkm_t{
    lista_t* lista_pokemon; // ? Cambiado a lista
    cola_t* lista_entrenadores;
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
bool ordenar_alfabeticamente(lista_t* lista_pokemon, size_t tope);

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
 * Post: Devuelve un puntero a entrenador si se pudo crear al entrenador y NULL en caso contrario
 */
entrenador_t* crear_entrenador(char* nombre, int id);

/*
 * Pre: nombre debe ser distinto de NULL
 * Post: Devuelve un puntero a pokemon si se pudo crear al pokemon y NULL en caso contrario
 */
pokemon_t* crear_pokemon(char* nombre, size_t nivel, int id_entrenador);  //! Corregir

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

/*
 * Pre: _pokemon debe ser un puntero a un pokemon_t
 * Post: Libera la memoria asignada a pokemon
 */
bool destructor_pokemon(void* _pokemon, void* aux);

/*
 * Pre: _entrenador debe ser un puntero a un entrenador_t
 * Post: Libera la memoria asignada a entrenador
 */
void destructor_entrenador(entrenador_t* entrenador);


void hospital_destruir_estructuras(hospital_t* hospital);

#endif // HOSPITAL_INTERNO_H_