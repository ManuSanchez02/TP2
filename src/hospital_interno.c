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

entrenador_t* entrenador_crear(char* nombre, int id){
        if(!nombre)
        return NULL;

    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if(!entrenador)
        return NULL;

    size_t tamanio = strlen(nombre)+1;
    entrenador->id = id;
    entrenador->nombre = malloc(tamanio);
    if(!entrenador->nombre){
        free(entrenador->nombre);
        return NULL;
    }
    memcpy(entrenador->nombre, nombre, tamanio);
    return entrenador;
}

pokemon_t* pokemon_crear(char* nombre, size_t nivel, entrenador_t* entrenador){
    if(!nombre || !entrenador)
        return NULL;

    pokemon_t* pokemon = malloc(sizeof(pokemon_t));
    if(!pokemon)
        return NULL;

    size_t tamanio = strlen(nombre)+1;
    pokemon->nivel = nivel;
    pokemon->entrenador = entrenador;
    pokemon->nombre = malloc(tamanio);
    if(!pokemon->nombre)
        return NULL;
    memcpy(pokemon->nombre, nombre, tamanio);
    return pokemon;
}

bool parsear_linea(hospital_t* hospital, char** elementos_linea){
    if(!hospital || !elementos_linea)
        return false;

    size_t i = 2;
    int id_entrenador = atoi(elementos_linea[POSICION_ID_ENTRENADOR]);

    entrenador_t* entrenador = entrenador_crear(elementos_linea[POSICION_NOMBRE_ENTRENADOR], id_entrenador);
    if(!entrenador)
        return false;

    while(elementos_linea[i] != NULL){
        pokemon_t* pokemon = pokemon_crear(elementos_linea[i], (size_t)atoi(elementos_linea[i+1]), entrenador);
        if(!pokemon)
            return false;

        if(!abb_insertar(hospital->lista_pokemon, pokemon))
            return false;

        i += POSICION_PRIMER_POKEMON;
    }

    if(!lista_insertar(hospital->lista_entrenadores, entrenador))
        return false;
    

    liberar_elementos_split(elementos_linea);
    free(elementos_linea);
    return true;
}

/*
 * Pre: -
 * Post: inserta elemento en lista y devuelve true si fue exitoso y false en caso contrario
 */
bool hospital_copiar_entrenadores_aux(void* elemento, void* lista){
    if(!lista || !elemento)
        return false;

    return lista_insertar(lista, elemento) != NULL;
}

/*
 * Pre: -
 * Post: inserta elemento en abb y devuelve true si fue exitoso y false en caso contrario
 */
bool hospital_copiar_pokemon_aux(void* elemento, void* abb){
    if(!elemento || !abb)
        return false;


    return abb_insertar(abb, elemento) != NULL;
}

bool hospital_copiar(hospital_t* hospital_copia, hospital_t* hospital_original){
    if(!hospital_copia || !hospital_original)
        return false;
    
    
    return (abb_con_cada_elemento(hospital_original->lista_pokemon, PREORDEN, hospital_copiar_pokemon_aux, hospital_copia->lista_pokemon) == abb_tamanio(hospital_original->lista_pokemon)
    && lista_con_cada_elemento(hospital_original->lista_entrenadores, hospital_copiar_entrenadores_aux, hospital_copia->lista_entrenadores) == lista_tamanio(hospital_original->lista_entrenadores));
}

void pokemon_destruir(void* _pokemon){
    if(!_pokemon)
        return;

    pokemon_t* pokemon = _pokemon;
    free(pokemon->nombre);
    free(pokemon);
}

bool entrenador_destruir(void* _entrenador, void* aux){
    if(!_entrenador)
        return false;

    entrenador_t* entrenador = _entrenador;

    free(entrenador->nombre);
    free(entrenador);
    return true;
}

void hospital_destruir_estructuras(hospital_t* hospital){
    if(!hospital)
        return;
    
    lista_destruir(hospital->lista_entrenadores);
    abb_destruir(hospital->lista_pokemon); 
    free(hospital);
}

int comparador_nombre_pokemon(void* _pokemon1, void* _pokemon2){
    if(!_pokemon1 || !_pokemon2)
        return 0;

    pokemon_t* pokemon1 = _pokemon1;
    pokemon_t* pokemon2 = _pokemon2;

    return strcmp(pokemon1->nombre, pokemon2->nombre);
}