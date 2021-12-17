#include "hospital.h"
#include "hospital_interno.h"

const char SEPARADOR = ';';


hospital_t* hospital_crear(){
    hospital_t* hospital = malloc(sizeof(hospital_t));
    if(!hospital)
        return NULL;

    hospital->lista_pokemon = abb_crear(comparador_nombre_pokemon);
    hospital->lista_entrenadores = lista_crear();
    if(!hospital->lista_pokemon || !hospital->lista_entrenadores){
        hospital_destruir(hospital);
        return NULL;
    }
    return hospital;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    FILE* entrenadores_y_pokemones = fopen(nombre_archivo, "r");
    if(!entrenadores_y_pokemones || !hospital)
        return false;
    
    char* linea = leer_linea(entrenadores_y_pokemones);
    char** elementos_linea = split(linea, SEPARADOR);
    if(!linea || !elementos_linea){
        free(linea);
        liberar_elementos_split(elementos_linea);
        fclose(entrenadores_y_pokemones);
        return false;
    }
    
    hospital_t* hospital_estado_inicial = hospital_crear();
    if(!hospital_copiar(hospital_estado_inicial, hospital)){
        hospital_destruir(hospital_estado_inicial);
        fclose(entrenadores_y_pokemones);
        return false;
    }

    while(linea[0] != '\0'){
        if(!parsear_linea(hospital, elementos_linea)){
            fclose(entrenadores_y_pokemones);
            hospital_destruir(hospital);
            hospital = hospital_estado_inicial;
            return false;
        }

        free(linea);
        linea = leer_linea(entrenadores_y_pokemones);
        elementos_linea = split(linea, SEPARADOR);
        if(!linea || !elementos_linea){
            fclose(entrenadores_y_pokemones);
            hospital_destruir(hospital);
            hospital = hospital_estado_inicial;
            return false;
        }
    }
    liberar_elementos_split(elementos_linea);
    free(linea);
    free(elementos_linea);
    hospital_destruir_estructuras(hospital_estado_inicial);

    fclose(entrenadores_y_pokemones);
    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    return (hospital) ? abb_tamanio(hospital->lista_pokemon) : 0;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    return (hospital) ? lista_tamanio(hospital->lista_entrenadores) : 0;
}

/*
 * Pre: _pokemon tiene que ser un puntero a un pokemon_t, y funcion un puntero a funcion que recibe un puntero a un pokemon_t y devuelve un bool.
 * Post: Devuelve false en caso de error o el retorno de la funcion pasada por parametro en caso contrario.
 */
bool hospital_a_cada_pokemon_aux(void* _pokemon, void* _funcion){
    if(!_pokemon || !_funcion)
        return false;

    bool (*funcion)(pokemon_t*) = _funcion;
    pokemon_t* pokemon = _pokemon;

    return funcion(pokemon);
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!hospital || !funcion)
        return 0;

    return abb_con_cada_elemento(hospital->lista_pokemon, INORDEN, hospital_a_cada_pokemon_aux, funcion);
}

void hospital_destruir(hospital_t* hospital){
    if(!hospital)
        return;
    
    lista_con_cada_elemento(hospital->lista_entrenadores, entrenador_destruir, NULL);
    lista_destruir(hospital->lista_entrenadores);
    abb_destruir_todo(hospital->lista_pokemon, pokemon_destruir);
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nivel : 0;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nombre : NULL;
}

entrenador_t* pokemon_entrenador(pokemon_t* pokemon){
    return (pokemon) ? pokemon->entrenador : NULL;
}
