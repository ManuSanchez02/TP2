#include "hospital.h"
#include "hospital_interno.h"


const char SEPARADOR = ';';


hospital_t* hospital_crear(){
    return calloc(1, sizeof(hospital_t));
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
    
    hospital_t* hospital_estado_incial = hospital_crear();
    if(!hospital_copiar(hospital_estado_incial, hospital)){
        hospital_destruir(hospital_estado_incial);
        fclose(entrenadores_y_pokemones);
        return false;
    }

    while(linea[0] != '\0'){
        if(!parsear_linea(hospital, elementos_linea)){
            fclose(entrenadores_y_pokemones);
            hospital_destruir(hospital);
            hospital = hospital_estado_incial;
            return false;
        }

        free(linea);
        linea = leer_linea(entrenadores_y_pokemones);
        elementos_linea = split(linea, SEPARADOR);
        if(!linea || !elementos_linea){
            fclose(entrenadores_y_pokemones);
            hospital_destruir(hospital);
            hospital = hospital_estado_incial;
            return false;
        }
    }
    liberar_elementos_split(elementos_linea);
    free(linea);
    free(elementos_linea);
    hospital_destruir(hospital_estado_incial);

    fclose(entrenadores_y_pokemones);
    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    return (hospital) ? hospital->cantidad_actual_pokemon : 0;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    return (hospital) ? hospital->cantidad_actual_entrenadores : 0;
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!hospital || !funcion)
        return 0;

    ordenar_alfabeticamente(hospital->vector_pokemon, hospital->cantidad_actual_pokemon);
    bool retorno_de_funcion = true;

    size_t contador = 0;
    while(contador < hospital->cantidad_actual_pokemon && retorno_de_funcion){
        retorno_de_funcion = funcion(&(hospital->vector_pokemon[contador]));
        contador++;
    }

    return contador;
}

void hospital_destruir(hospital_t* hospital){
    if(!hospital)
        return;
    
    for(int i = 0; i < hospital_cantidad_entrenadores(hospital); i++){
        free(hospital->vector_entrenadores[i].nombre);
    }

    for(int i = 0; i < hospital_cantidad_pokemon(hospital); i++){
        free(hospital->vector_pokemon[i].nombre);
    }

    free(hospital->vector_entrenadores);
    free(hospital->vector_pokemon);
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nivel : 0;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nombre : NULL;
}