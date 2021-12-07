#include "hospital.h"
#include "hospital_interno.h"


const char SEPARADOR = ';';


hospital_t* hospital_crear(){
    hospital_t* hospital = calloc(1, sizeof(hospital_t));
    if(!hospital)
        return NULL;

    hospital->vector_pokemon = lista_crear();
    hospital->vector_entrenadores = lista_crear();
    if(!hospital->vector_pokemon || !hospital->vector_entrenadores){
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
    hospital_destruir(hospital_estado_inicial);

    fclose(entrenadores_y_pokemones);
    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    return (hospital) ? lista_tamanio(hospital->vector_pokemon) : 0;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    return (hospital) ? lista_tamanio(hospital->vector_entrenadores) : 0;
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!hospital || !funcion)
        return 0;

    if(!ordenar_alfabeticamente(hospital->vector_pokemon, hospital_cantidad_pokemon(hospital)))
        return 0;
    bool retorno_de_funcion = true;

    size_t contador = 0;
    while(contador < hospital_cantidad_pokemon(hospital) && retorno_de_funcion){
        retorno_de_funcion = funcion(lista_elemento_en_posicion(hospital->vector_pokemon, contador));
        contador++;
    }

    return contador;
}

void hospital_destruir(hospital_t* hospital){
    if(!hospital)
        return;
    
    lista_con_cada_elemento(hospital->vector_entrenadores, destructor_entrenador, NULL);
    lista_con_cada_elemento(hospital->vector_pokemon, destructor_pokemon, NULL);

    lista_destruir(hospital->vector_entrenadores);
    lista_destruir(hospital->vector_pokemon); 
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nivel : 0;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    return (pokemon) ? pokemon->nombre : NULL;
}
