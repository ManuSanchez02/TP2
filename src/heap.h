#ifndef HEAP_H_
#define HEAP_H_

typedef struct _heap_t heap_t;

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

/*
 * Pre: tamanio_inicial debe ser mayor a 0 y comparador debe ser una funcion de comparacion
 * Post: Crea y devuelve un heap con una capacidad inicial de tamanio_inicial. Si falla, devuelve NULL
 */
heap_t* heap_crear(size_t tamanio_inicial, int (*comparador)(void*, void*));

/*
 * Pre: -
 * Post: inserta el elemento en la posicion respectiva del heap, aumenta el tamanio en 1 y devuelve un puntero al heap. Si falla, devuelve NULL.
 */
void* heap_insertar(heap_t* heap, void* elemento);

/*
 * Pre: -
 * Post: extrae el elemento minimo del heap, lo sustituye por un nuevo minimo, y lo devuelve. Si falla deveulve NULL.
 */
void* heap_extraer_raiz(heap_t* heap);

/*
 * Pre: -
 * Post: devuelve true si el heap esta vacio, o false en caso contrario. Si heap es NULL, devuelve false.
 */
bool heap_vacio(heap_t* heap);

/*
 * Pre: -
 * Post: devuelve el tamaño del heap. Si heap es NULL, devuelve 0.
 */
int heap_tamanio(heap_t* heap);

/*
 * Pre: -
 * Post: libera la memoria reservada para el heap.
 */
void heap_destruir(heap_t* heap);

#endif // HEAP_H_