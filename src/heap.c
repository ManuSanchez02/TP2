#include "heap.h"

struct _heap_t{
    void** vector;
    size_t tope;
    size_t tamanio_max;
    int (*comparador)(void*, void*);
};

/*
 * Pre: pos1 y pos2 deben ser mayores o iguales a 0
 * Post: intercambia el elemento en la posicion pos1 con el elemento en la posicion pos2 de vector
 */
void swap(void** vector, int pos1, int pos2){
    if(!vector)
        return;

    void* aux = vector[pos1];
    vector[pos1] = vector[pos2];
    vector[pos2] = aux;
}


heap_t* heap_crear(size_t tamanio_inicial, int (*comparador)(void*, void*)){
    if(tamanio_inicial == 0 || !comparador)
        return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap)
        return NULL;
    
    heap->tope = 0;
    heap->tamanio_max = tamanio_inicial;
    heap->comparador = comparador;
    heap->vector = malloc(sizeof(void*)*tamanio_inicial);
    if(!heap->vector){
        free(heap);
        return NULL;
    }

    return heap;
}


/*
 * Pre: pos_actual debe ser mayor a 0 y menor o igual a tope
 * Post: si el elemento en la posicion pos_actual es menor que el elemento en la posicion padre, intercambia los elementos hasta que esta condicion no se cumpla
 */
void sift_up(heap_t* heap, int pos_actual){
    if(!heap)
        return;
    
    void** vector = heap->vector;

    int pos_padre;
    
    if(pos_actual % 2 == 0)
        pos_padre = (pos_actual-2)/2;
    else
        pos_padre = (pos_actual-1)/2;

    if(pos_padre < 0)
        return;

    if(heap->comparador(vector[pos_actual], vector[pos_padre]) < 0){
        swap(vector, pos_actual, pos_padre);
        sift_up(heap, pos_padre);
    }
}


void* heap_insertar(heap_t* heap, void* elemento){
    if(!heap)
        return NULL;
    
    if(heap->tamanio_max == heap->tope){
        void** vector_aux = realloc(heap->vector, (heap->tamanio_max*2)*sizeof(void*));
        if(!vector_aux)
            return NULL;
        
        heap->vector = vector_aux;
        heap->tamanio_max *= 2;
    }
    
    heap->vector[heap->tope] = elemento;
    sift_up(heap, (int)heap->tope);
    heap->tope += 1;
    return heap;
}

/*
 * Pre: -
 * Post: si el elemento en la posicion pos_actual es mayor que el menor de sus hijos, intercambia los elementos hasta que esta condicion no se cumpla
 */
void sift_down(heap_t* heap, int pos_actual){
    if(!heap || pos_actual >= heap->tope || pos_actual*2+1 >= heap->tope)
        return;
    
    
    int pos_izq = 2*pos_actual+1, pos_der = 2*pos_actual+2;
    int pos_a_comparar;


    if(pos_der >= heap->tope){
        pos_a_comparar = pos_izq;
    }else{
        pos_a_comparar = heap->comparador(heap->vector[pos_izq], heap->vector[pos_der]) < 0 ? pos_izq : pos_der;
    }
    
    if(heap->comparador(heap->vector[pos_a_comparar], heap->vector[pos_actual]) < 0){
        swap(heap->vector, pos_a_comparar, pos_actual);
        sift_down(heap, pos_a_comparar);
    }    
}

void* heap_extraer_raiz(heap_t* heap){
    if(!heap || heap_vacio(heap))
        return NULL;

    void* raiz = heap->vector[0];
    heap->vector[0] = heap->vector[--(heap->tope)];

    sift_down(heap, 0);

    return raiz;
}

bool heap_vacio(heap_t* heap){
    if(!heap)
        return false;

    return heap_tamanio(heap) == 0;
}

int heap_tamanio(heap_t* heap){
    if(!heap)
        return -1;

    return (int)heap->tope;
}

void heap_destruir(heap_t* heap){
    if(!heap)
        return;
    
    free(heap->vector);
    free(heap);
}

/*
 * Pre: elem1 y elem2 deben ser punteros a enteros
 * Post: devuelve un numero mayor a 0 si elem1 es mayor que elem2, 0 si son iguales y un numero menor a 0 si elem1 es menor que elem2
 */
int entero_comp(void* elem1, void* elem2){
    if(!elem1 || !elem2)
        return 0;

    return *(int*)elem1 - *(int*)elem2;
}