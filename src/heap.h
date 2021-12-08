#ifndef HEAP_H_
#define HEAP_H_

typedef struct _heap_t heap_t;

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

heap_t* heap_crear();

bool heap_insertar(heap_t* heap, void* elemento);

void* heap_extraer_raiz(heap_t* heap);

bool heap_vacio(heap_t* heap);

void heap_destruir(heap_t* heap);

#endif // HEAP_H_