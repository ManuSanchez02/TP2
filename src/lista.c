#include "lista.h"
#include <stddef.h>
#include <stdlib.h>


lista_t* lista_crear(){
    return calloc(1, sizeof(lista_t));
}

lista_t* lista_insertar(lista_t* lista, void* elemento){
    if(!lista)
        return NULL;
    
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo)
        return NULL;

    nodo->elemento = elemento;
    nodo->siguiente = NULL;

    if(lista_vacia(lista)){
        lista->nodo_inicio = nodo;
    }else{
        lista->nodo_fin->siguiente = nodo;
    }

    lista->nodo_fin = nodo;
    lista->cantidad++;
    
    return lista;
}

lista_t* lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista)
        return NULL;

    if(lista_vacia(lista)){
        lista_t* lista_aux = lista_insertar(lista, elemento);
        if(!lista_aux)
            return NULL;

        lista = lista_aux;
        return lista;
    }

    nodo_t* actual = lista->nodo_inicio;
    nodo_t* nuevo = malloc(sizeof(nodo_t));
    if(!nuevo)
        return NULL;
    nuevo->elemento = elemento;

    if(posicion == 0){
        nuevo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo;

    }else{
        int i = 1;

        while(actual->siguiente && i < posicion){
            actual = actual->siguiente;
            i++;
        }

        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }    

    if(posicion >= lista_tamanio(lista)){
        lista->nodo_fin = nuevo;
    }
        
    lista->cantidad++;
    return lista;
}

void* lista_quitar(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return NULL;

    void* elemento_ultimo = NULL;
    

    if(lista_tamanio(lista) == 1){
        elemento_ultimo = lista->nodo_inicio->elemento;
        free(lista->nodo_inicio);
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;

    }else{
        nodo_t* nuevo_ultimo = lista->nodo_inicio;
        while(nuevo_ultimo->siguiente != lista->nodo_fin)
            nuevo_ultimo = nuevo_ultimo->siguiente;

        elemento_ultimo = lista->nodo_fin->elemento;
        free(lista->nodo_fin);
        lista->nodo_fin = nuevo_ultimo;
        lista->nodo_fin->siguiente = NULL;
    }

    lista->cantidad--;

    return elemento_ultimo;
}

void* lista_quitar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista))
        return NULL;

    if(lista_tamanio(lista) == 1 || posicion >= lista_tamanio(lista)-1)
        return lista_quitar(lista);


    void* elemento_borrado = NULL;
    nodo_t* a_eliminar;

    if(posicion == 0){
        a_eliminar = lista->nodo_inicio;
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
        elemento_borrado = a_eliminar->elemento;
        
    }else{
        nodo_t* anterior = lista->nodo_inicio;
        a_eliminar = lista->nodo_inicio->siguiente;
        int i = 1;
        while(i < posicion){
            anterior = anterior->siguiente;
            a_eliminar = a_eliminar->siguiente;
            i++;
        }

        anterior->siguiente = a_eliminar->siguiente;
        elemento_borrado = a_eliminar->elemento;
        
    }
    
    free(a_eliminar);
    lista->cantidad--;
    
    return elemento_borrado;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(!lista || posicion >= lista->cantidad)
        return NULL;
    
    nodo_t* nodo_actual = lista->nodo_inicio;

    for(size_t i = 0; i < posicion; i++){
        nodo_actual = nodo_actual->siguiente;
    }

    return nodo_actual->elemento;
}

void* lista_primero(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return NULL;

    return lista->nodo_inicio->elemento;
}

void* lista_ultimo(lista_t* lista){
    if(!lista || lista_vacia(lista))
        return NULL;

    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if(!lista)
        return true;

    if(lista->cantidad == 0)
        return true;
    
    return false;
}

size_t lista_tamanio(lista_t* lista){
    if(!lista || lista_vacia(lista)){
        return 0;
    }

    return lista->cantidad;
}

/*
 *  Pre:
 *  Post: Libera la memoria asignada al nodo pasado por parametro y la de todos los nodos siguientes al mismo
 */
void nodo_destruir(nodo_t* nodo){
    if(!nodo)
        return;

    nodo_destruir(nodo->siguiente);
    free(nodo);
}

void lista_destruir(lista_t* lista){
    if(!lista)
        return;

    nodo_destruir(lista->nodo_inicio);
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista)
        return NULL;

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if(!iterador)
        return NULL;

    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente)
        return false;

    return true;    
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(!iterador || !lista_iterador_tiene_siguiente(iterador))
        return false;

    iterador->corriente = iterador->corriente->siguiente;
    return (iterador->corriente != NULL);
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if(!iterador || !iterador->corriente)
        return NULL;

    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if(!lista || !funcion || lista_vacia(lista))
        return 0;

    size_t elementos_iterados = 0;
    nodo_t* actual = lista->nodo_inicio;

    while(actual && funcion(actual->elemento, contexto)){
        elementos_iterados++;
        actual = actual->siguiente;
    }
    
    return elementos_iterados;
}
