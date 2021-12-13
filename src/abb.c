#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct vector{
  void** elementos;
  size_t tamanio_maximo;
  size_t tamanio_actual;
} vector_t;

abb_t* abb_crear(abb_comparador comparador){
    abb_t* arbol = calloc(1, sizeof(abb_t));
    if(!arbol)
      return NULL;

    arbol->comparador = comparador;
    return arbol;
}

nodo_abb_t* nodo_abb_crear(void* elemento){
  nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));
  if(!nodo)
    return NULL;

  nodo->elemento = elemento;
  return nodo;
}

/*
 * Pre: nodo_actual es el nodo desde donde comienza la insercion.
 * Post: Inserta nodo_nuevo en la posicion correspondiente respecto de los hijos de nodo_actual.
 */
nodo_abb_t* abb_insertar_aux(nodo_abb_t* nodo_actual, nodo_abb_t* nodo_nuevo, abb_comparador comparador){
  if(!nodo_actual){
    return nodo_nuevo;
  }

  int comparacion = comparador(nodo_actual->elemento, nodo_nuevo->elemento);

  if(comparacion > 0)
    nodo_actual->izquierda = abb_insertar_aux(nodo_actual->izquierda, nodo_nuevo, comparador);
  else
    nodo_actual->derecha = abb_insertar_aux(nodo_actual->derecha, nodo_nuevo, comparador);

  return nodo_actual;
}

abb_t* abb_insertar(abb_t* arbol, void* elemento){
  if(!arbol)
    return NULL;

  nodo_abb_t* nodo_nuevo = nodo_abb_crear(elemento);
  if(!nodo_nuevo)
    return NULL;
  
  arbol->nodo_raiz = abb_insertar_aux(arbol->nodo_raiz, nodo_nuevo, arbol->comparador);
  arbol->tamanio++;
  return arbol;
}

/*
 * Pre: -
 * Post: Guarda el nodo mas derecho en nodo_mas_derecho y lo sustituye por su hijo izquierdo.
 */ 
nodo_abb_t* extraer_nodo_mas_derecho(nodo_abb_t* nodo_actual, nodo_abb_t** nodo_mas_derecho){
  if(!nodo_actual)
    return NULL;

  if(!nodo_actual->derecha){
    *nodo_mas_derecho = nodo_actual;
    return nodo_actual->izquierda;
  }

  nodo_actual->derecha = extraer_nodo_mas_derecho(nodo_actual->derecha, nodo_mas_derecho);
  return nodo_actual;
}

/*
 * Pre: - El tamanio del arbol debe ser correcto.
 *      - El nodo_actual es el nodo desde donde comienza la busqueda del elemento a quitar.
 * Post: Guarda en elemento_eliminado el puntero al elemento quitado del arbol.
 */
nodo_abb_t* abb_quitar_aux(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador, void** elemento_eliminado, size_t* tamanio_arbol){
  if(!nodo_actual || !comparador || !tamanio_arbol)
    return NULL;

  int comparacion = comparador(nodo_actual->elemento, elemento);

  if(comparacion == 0){
    *elemento_eliminado = nodo_actual->elemento;
    *tamanio_arbol -= 1;


    if(nodo_actual->izquierda && nodo_actual->derecha){
      nodo_abb_t* nodo_mas_derecho = NULL;
      nodo_actual->izquierda = extraer_nodo_mas_derecho(nodo_actual->izquierda, &nodo_mas_derecho);

      nodo_actual->elemento = nodo_mas_derecho->elemento;
      free(nodo_mas_derecho);
      return nodo_actual;

    }else{
      nodo_abb_t* hijo = nodo_actual->izquierda ? nodo_actual->izquierda : nodo_actual->derecha;
      free(nodo_actual);

      return hijo;
    }

  }else if(comparacion > 0){
    nodo_actual->izquierda = abb_quitar_aux(nodo_actual->izquierda, elemento, comparador, elemento_eliminado, tamanio_arbol);

  }else{
    nodo_actual->derecha = abb_quitar_aux(nodo_actual->derecha, elemento, comparador, elemento_eliminado, tamanio_arbol);

  }

  return nodo_actual;
}

void* abb_quitar(abb_t* arbol, void *elemento){
  if(!arbol)
    return NULL;

  void* elemento_eliminado = NULL;
  arbol->nodo_raiz = abb_quitar_aux(arbol->nodo_raiz, elemento, arbol->comparador, &elemento_eliminado, &arbol->tamanio);

  return elemento_eliminado;
}

/*
 * Pre: nodo_actual es el nodo desde donde comienza la busqueda.
 * Post: Devuelve el elemento buscado si es que existe. En caso contrario devuelve NULL.
*/
void* abb_buscar_aux(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador){
  if(!nodo_actual)
    return NULL;

  int comparacion = comparador(nodo_actual->elemento, elemento);

  if(comparacion == 0)
    return nodo_actual->elemento;
  else if(comparacion > 0)
    return abb_buscar_aux(nodo_actual->izquierda, elemento, comparador);
  else
    return abb_buscar_aux(nodo_actual->derecha, elemento, comparador);
}

void* abb_buscar(abb_t* arbol, void* elemento){
  if(!arbol)
    return NULL;

  return abb_buscar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t* arbol){
  if(!arbol)
    return true;

  return (arbol->tamanio == 0);
}

size_t abb_tamanio(abb_t *arbol){
  if(!arbol)
    return 0;
  
  return arbol->tamanio;
}

/*
 * Pre: nodo_actual es el nodo desde donde comienza la destruccion.
 * Post: Destruye el arbol, sus nodos y sus elementos (solo si se pasa un destructor).
*/
void abb_destruir_aux(nodo_abb_t* nodo_actual, void (*destructor)(void *)){
  if(!nodo_actual)
    return;

  abb_destruir_aux(nodo_actual->izquierda, destructor);
  abb_destruir_aux(nodo_actual->derecha, destructor);

  if(destructor)
    destructor(nodo_actual->elemento);

  free(nodo_actual);
}

void abb_destruir(abb_t *arbol){
  if(!arbol)
    return;

  abb_destruir_aux(arbol->nodo_raiz, NULL);
  free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *)){
  if(!arbol || !destructor)
    return;

  abb_destruir_aux(arbol->nodo_raiz, destructor);
  free(arbol);
}

/*
 * Pre: - nodo_actual debe ser el nodo desde donde se aplica la funcion.
 *      - *estado debe ser inicializado en true
 * Post: Recorre el arbol aplicandole la funcion a cada elemento hasta que la funcion devuelva false. Devuelve la cantidad de invocaciones de la funcion.
 */ 
size_t abb_con_cada_elemento_aux(nodo_abb_t* nodo_actual, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux, bool* estado){
  if(!nodo_actual || !funcion || !estado)
    return 0;

  size_t cantidad = 0;

  if(recorrido == PREORDEN){
    if(!funcion(nodo_actual->elemento, aux)){
      *estado = false;
      return 1;
    }

    cantidad = 1 + abb_con_cada_elemento_aux(nodo_actual->izquierda, PREORDEN, funcion, aux, estado);

    if(*estado == false){
      return cantidad;
    }

    cantidad += abb_con_cada_elemento_aux(nodo_actual->derecha, PREORDEN, funcion, aux, estado);

    return cantidad;
  }


  if(recorrido == INORDEN){
    cantidad = abb_con_cada_elemento_aux(nodo_actual->izquierda, INORDEN, funcion, aux, estado);

    if(*estado == false){
      return cantidad;
    }

    if(!funcion(nodo_actual->elemento, aux)){
      *estado = false;
      return cantidad + 1;
    }

    cantidad += abb_con_cada_elemento_aux(nodo_actual->derecha, INORDEN, funcion, aux, estado);

    return cantidad + 1;
  }


  if(recorrido == POSTORDEN){
    cantidad = abb_con_cada_elemento_aux(nodo_actual->izquierda, POSTORDEN, funcion, aux, estado);

    if(*estado == false){
      return cantidad;
    }

    cantidad += abb_con_cada_elemento_aux(nodo_actual->derecha, POSTORDEN, funcion, aux, estado);

    if(*estado == false){
      return cantidad;
    }

    if(!funcion(nodo_actual->elemento, aux)){
      *estado = false;
      return cantidad + 1;
    }

    return cantidad + 1;
  }

  return 0;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux){
  if(!arbol || !funcion)
    return 0;
  
  bool estado = true;
  return abb_con_cada_elemento_aux(arbol->nodo_raiz, recorrido, funcion, aux, &estado);
}

/*
 * Pre: -
 * Post: Agrega elemento a vector->elementos aumentando la cantidad de elementos del mismo.
 */ 
bool agregar_a_vector(void* elemento, void* vector){
  if(!vector)
    return NULL;
  
  vector_t* vector_aux = (vector_t*) vector;
  if(!vector_aux->elementos || vector_aux->tamanio_actual >= vector_aux->tamanio_maximo)
    return false;

  vector_aux->elementos[vector_aux->tamanio_actual] = elemento;
  vector_aux->tamanio_actual++;

  return true;
}

size_t abb_recorrer(abb_t* arbol, abb_recorrido recorrido, void** array, size_t tamanio_array){
  if(!arbol || !array)
    return 0;

  vector_t vector;
  vector.elementos = array;
  vector.tamanio_maximo = tamanio_array;
  vector.tamanio_actual = 0;

  abb_con_cada_elemento(arbol, recorrido, agregar_a_vector, &vector);

  return vector.tamanio_actual;
}