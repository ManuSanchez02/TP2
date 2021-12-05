#include "hospital_interno.h"


const size_t MULTIPLICADOR_ALLOC = 10;

#define POSICION_ID_ENTRENADOR 0
#define POSICION_NOMBRE_ENTRENADOR 1
#define POSICION_PRIMER_POKEMON 2

char* leer_linea(FILE* archivo){
    size_t longitud = 0;
    size_t maximo = 1;
    char* linea = malloc(sizeof(char));
    if(!linea)
        return NULL;
    
    char caracter_leido = (char)fgetc(archivo);
    while(caracter_leido != '\n' && caracter_leido != EOF){

        if(longitud == maximo){
            maximo *= 2;
            char* linea_aux = realloc(linea, maximo*sizeof(char));
            if(!linea_aux){
                free(linea);
                return NULL;
            }
            linea = linea_aux;
        }

        linea[longitud] = caracter_leido;
        caracter_leido = (char)fgetc(archivo);
        
        longitud++;
    }
    linea[longitud] = '\0';

    return linea;
}

void ordenar_alfabeticamente(pokemon_t* vector, size_t tope){
    if(!vector)
        return;
        
    for(size_t i = 0; i < tope; i++){
        for(size_t j = 0; j < tope-i-1; j++){
            if(strcmp(vector[j].nombre, vector[j+1].nombre) > 0){
                pokemon_t aux = vector[j];
                vector[j] = vector[j+1];
                vector[j+1] = aux;
            }
        }
    }
}

entrenador_t* agregar_entrenador(entrenador_t* vector, entrenador_t elemento, size_t* cantidad_elementos, size_t* cantidad_maxima){   
    if(!vector){
        vector = malloc(sizeof(entrenador_t));
        if(!vector)
            return NULL;
        
        *cantidad_maxima = 1;
    }

    if((*cantidad_elementos) == (*cantidad_maxima)){
        void* vector_aux = realloc(vector, sizeof(entrenador_t)*(*cantidad_maxima)*2);
        if(!vector_aux)
            return NULL;

        *cantidad_maxima *= 2;
        vector = vector_aux;
    }
    
    vector[*cantidad_elementos] = elemento;
    (*cantidad_elementos)++;
    return vector;
}

pokemon_t* agregar_pokemon(pokemon_t* vector, pokemon_t elemento, size_t* cantidad_elementos, size_t* cantidad_maxima){  
    if(!vector){
        vector = malloc(sizeof(pokemon_t));
        if(!vector)
            return NULL;
        
        *cantidad_maxima = 1;
    }

    if((*cantidad_elementos) == (*cantidad_maxima)){
        void* vector_aux = realloc(vector, sizeof(pokemon_t)*(*cantidad_maxima)*2);
        if(!vector_aux)
            return NULL;

        *cantidad_maxima *= 2;
        vector = vector_aux;
    }
    
    vector[*cantidad_elementos] = elemento;
    (*cantidad_elementos)++;
    return vector;
}

bool inicializar_entrenador(entrenador_t* entrenador, int id, char* nombre){
    if(!nombre)
        return false;

    size_t tamanio = sizeof(char)*(strlen(nombre)+1);
    entrenador->id = id;
    entrenador->nombre = malloc(tamanio);
    if(!entrenador->nombre)
        return false;
    strncpy(entrenador->nombre, nombre, tamanio);
    return true;
}

bool inicializar_pokemon(pokemon_t* pokemon, char* nombre, size_t nivel){
    if(!nombre)
        return false;

    size_t tamanio = sizeof(char)*(strlen(nombre)+1);
    pokemon->nivel = nivel;
    pokemon->nombre = malloc(tamanio);
    if(!pokemon->nombre)
        return false;
    strncpy(pokemon->nombre, nombre, tamanio);
    return true;
}

bool parsear_linea(hospital_t* hospital, char** elementos_linea){
    if(!hospital || !elementos_linea)
        return false;

    size_t i = 2;
        while(elementos_linea[i] != NULL){
            pokemon_t pokemon;
            bool pokemon_inicializado = inicializar_pokemon(&pokemon, elementos_linea[i], (size_t)atoi(elementos_linea[i+1]));
            if(!pokemon_inicializado)
                return false;

            pokemon_t* vector_pokemon_aux = agregar_pokemon(hospital->vector_pokemon, pokemon, &(hospital->cantidad_actual_pokemon), &(hospital->cantidad_maxima_pokemon));
            if(!vector_pokemon_aux)
                return false;

            hospital->vector_pokemon = vector_pokemon_aux;
            i += POSICION_PRIMER_POKEMON;
        }

        entrenador_t entrenador;
        bool entrenador_inicializado = inicializar_entrenador(&entrenador, atoi(elementos_linea[POSICION_ID_ENTRENADOR]), elementos_linea[POSICION_NOMBRE_ENTRENADOR]);
        if(!entrenador_inicializado)
            return false;

        entrenador_t* vector_entrenadores_aux = agregar_entrenador(hospital->vector_entrenadores, entrenador, &(hospital->cantidad_actual_entrenadores), &(hospital->cantidad_maxima_entrenadores));
        if(!vector_entrenadores_aux)
            return false;

        hospital->vector_entrenadores = vector_entrenadores_aux;
        
    
        liberar_elementos_split(elementos_linea);
        free(elementos_linea);
        return true;
}

bool hospital_copiar(hospital_t* hospital_copia, hospital_t* hospital_original){
    if(!hospital_copia || !hospital_original)
        return false;

    for(int i = 0; i < hospital_copia->cantidad_actual_pokemon; i++){
        pokemon_t* vector_pokemon_aux = agregar_pokemon(hospital_copia->vector_pokemon, hospital_original->vector_pokemon[i], &(hospital_copia->cantidad_actual_pokemon), &(hospital_copia->cantidad_maxima_pokemon));
        if(!vector_pokemon_aux)
            return false;
        i++;
        hospital_copia->vector_pokemon = vector_pokemon_aux;
    }

    for(int i = 0; i < hospital_copia->cantidad_maxima_entrenadores; i++){
        entrenador_t* vector_entrenadores_aux = agregar_entrenador(hospital_copia->vector_entrenadores, hospital_original->vector_entrenadores[i], &(hospital_copia->cantidad_maxima_entrenadores), &(hospital_copia->cantidad_maxima_pokemon));
        if(!vector_entrenadores_aux)
            return false;
        i++;
        hospital_copia->vector_entrenadores = vector_entrenadores_aux;
    }

    return true;
}