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

bool ordenar_alfabeticamente(lista_t* pokemones, size_t tope){
    if(!pokemones)
        return false;
    
    bool error = false;

    for(size_t i = 0; i < tope; i++){
        for(size_t j = 0; j < tope-i-1; j++){
            if(strcmp((*(pokemon_t*)lista_elemento_en_posicion(pokemones, j)).nombre, (*(pokemon_t*)lista_elemento_en_posicion(pokemones, j+1)).nombre) > 0){
                pokemon_t* aux = lista_quitar_de_posicion(pokemones, j);
                if(!lista_insertar_en_posicion(pokemones, aux, j+1))
                    error = true;
            }
        }
    }

    return !error;
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

entrenador_t* crear_entrenador(char* nombre, int id){
        if(!nombre)
        return NULL;

    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if(!entrenador)
        return NULL;

    size_t tamanio = sizeof(char)*(strlen(nombre)+1);
    entrenador->id = id;
    entrenador->nombre = malloc(tamanio);
    if(!entrenador->nombre){
        free(entrenador->nombre);
        return NULL;
    }
    strncpy(entrenador->nombre, nombre, tamanio);
    return entrenador;
}

pokemon_t* crear_pokemon(char* nombre, size_t nivel, int id_entrenador){
    if(!nombre)
        return NULL;

    pokemon_t* pokemon = malloc(sizeof(pokemon_t));
    if(!pokemon)
        return NULL;

    size_t tamanio = sizeof(char)*(strlen(nombre)+1);
    pokemon->nivel = nivel;
    pokemon->id_entrenador = id_entrenador;
    pokemon->nombre = malloc(tamanio);
    if(!pokemon->nombre)
        return NULL;
    strncpy(pokemon->nombre, nombre, tamanio);
    return pokemon;
}

bool parsear_linea(hospital_t* hospital, char** elementos_linea){
    if(!hospital || !elementos_linea)
        return false;

    size_t i = 2;
    int id_entrenador = atoi(elementos_linea[POSICION_ID_ENTRENADOR]);

    entrenador_t* entrenador = crear_entrenador(elementos_linea[POSICION_NOMBRE_ENTRENADOR], id_entrenador);
    if(!entrenador)
        return false;

    while(elementos_linea[i] != NULL){
        pokemon_t* pokemon = crear_pokemon(elementos_linea[i], (size_t)atoi(elementos_linea[i+1]), id_entrenador);
        if(!pokemon)
            return false;

        if(!lista_insertar(hospital->lista_pokemon, pokemon))
            return false;

        i += POSICION_PRIMER_POKEMON;
    }

    if(!cola_encolar(hospital->lista_entrenadores, entrenador))
        return false;
    

    liberar_elementos_split(elementos_linea);
    free(elementos_linea);
    return true;
}

bool hospital_copiar(hospital_t* hospital_copia, hospital_t* hospital_original){
    if(!hospital_copia || !hospital_original)
        return false;

    for(size_t i = 0; i < lista_tamanio(hospital_original->lista_pokemon); i++){
        pokemon_t* pokemon = lista_elemento_en_posicion(hospital_original->lista_pokemon, i);
        if(!lista_insertar(hospital_copia->lista_pokemon, pokemon))
            return false;
    }

    for(size_t i = 0; i < cola_tamanio(hospital_original->lista_entrenadores); i++){
        entrenador_t* entrenador = cola_desencolar(hospital_original->lista_entrenadores); //! Explicar xq desencolo y encolo
        if(!cola_encolar(hospital_original->lista_entrenadores, entrenador) || !cola_encolar(hospital_copia->lista_entrenadores, entrenador))
            return false;
    }

    return true;
}


bool destructor_pokemon(void* _pokemon, void* aux){
    if(!_pokemon)
        return true;

    pokemon_t* pokemon = _pokemon;
    free(pokemon->nombre);
    free(pokemon);
    return true;
}

void destructor_entrenador(entrenador_t* entrenador){
    if(!entrenador)
        return;

    free(entrenador->nombre);
    free(entrenador);
}

void hospital_destruir_estructuras(hospital_t* hospital){
    if(!hospital)
        return;
    
    cola_destruir(hospital->lista_entrenadores);
    lista_destruir(hospital->lista_pokemon); 
    free(hospital);
}
