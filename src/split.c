#include"split.h"
#include<stdlib.h>
#include<stdbool.h>

const char ERROR = -1;


void liberar_elementos_split(char** elementos){
    if(!elementos)
        return;
    int i = 0;
    while(elementos[i] != NULL){
        free(elementos[i]);
        i++;
    }
}

/*
 * Pre: posicion_actual debe ser mayor o igual a 0;
 * Post: Devuelve la posicion del proximo separador o del '\0' si es que no hay mas separadores. Si el string es invalido o posicion_actual < 0, devuelve ERROR; 
 */
int proximo_separador(const char* string, int posicion_actual, char separador){
    if(!string || posicion_actual < 0){
        return ERROR;
    }
    int i = posicion_actual;
    int posicion_separador = ERROR;

    while(string[i] != '\0' && string[i] != separador){
        i++;
    }

    if(string[i] == separador || string[i] == '\0'){
        posicion_separador = i;
    }

    return posicion_separador;
}

/*
 * Pre: string debe estar inicializado y ser distinto de NULL.
 * Post: -
 */
size_t cantidad_separadores(const char* string, char separador){
    size_t cantidad = 0;
    int i = 0;

    while(string[i] != '\0'){
        if(string[i] == separador){
            cantidad++;
        }
        i++;
    }

    return cantidad;
}


/*
 * Pre: vector tiene que estar inicializado y ser distinto de NULL
 * Post: - Aumenta el tope en uno
 *       - Devuelve un puntero al nuevo vector con el elemento agregado, liberando el anterior
 *       - Si falla devuelve NULL
 */
char** agregar_a_vector(char** vector, void* elemento, int* tope){
    char** vector_nuevo = (char**)realloc(vector, (size_t) (*tope+2)*sizeof(char*));
    if(!vector_nuevo){
        liberar_elementos_split(vector);
        free(vector);
        return NULL;
    }
    
    vector_nuevo[*tope] = elemento;
    vector_nuevo[(*tope)+1] = NULL;
    (*tope)++;
    return vector_nuevo;
}


char** split(const char* string, char separador){
    if(!string){
        return NULL;
    }
    int tope = 0;
    int i = 0,
        j = 0;
    size_t cantidad_substrings = cantidad_separadores(string, separador)+1;

    char** vector = malloc(cantidad_substrings*sizeof(char*));
    char* substring = malloc((size_t) (proximo_separador(string, 0, separador)+1)*sizeof(char));
    if(!substring || !vector){
        free(vector);
        free(substring);
        return NULL;
    }

    while(string[i] != '\0'){
        if(string[i] == separador){
            substring[j] = '\0';
            
            vector = agregar_a_vector(vector, substring, &tope);
            substring = malloc((size_t)(proximo_separador(string, i+1, separador)-i)*sizeof(char));
            if(!substring || !vector){
                liberar_elementos_split(vector);
                free(vector);
                free(substring);
                return NULL;
            }
            j = 0;
            
        }else{
            substring[j] = string[i];
            j++;
        }
        i++;
    }

    substring[j] = '\0';
    vector = agregar_a_vector(vector, substring, &tope);

    return vector;
}