![](https://i.imgur.com/P0aqOMI.jpg)

# **Trabajo Práctico 2 — Simulador Pokemon** 


## [7541/9515] Algoritmos y Programación II


### Segundo cuatrimestre de 2021

| Alumno:           | SANCHEZ FERNANDEZ DE LA VEGA, Manuel |
| ----------------- | ---------------- |
| Número de padrón: | 107951           |
| Email:            | msanchezf@fi.uba.ar |


#### 1. Introducción

La idea general del TP era realizar un simulador haciendo uso de los pasados trabajos practicos. El TP2 se basaba en el hospital del TP1. Sin embargo, habia que hacer cambios a la implementacion debido a la restriccion de no usar vectores, solo TDAs. En consecuencia, tambien tuve que usar los TDAs que desarrolle durante el año teniendo en cuenta cual es el mas optimo para usar en cada caso.

Luego de desarrollar el simulador, tuve que implementar al mismo en un pequeño juego utilizando la linea de comandos.

#### 2. Detalles de implementación


Antes de implementar el TP2, tuve que hacer cambios a las estructuras del hospital del TP1. Para empezar, reemplace el vector de pokemon por un arbol binario. Esto se debe a que me viene muy bien a la hora de implementar la funcion de `lista_con_cada_elemento()` ya que esta recorre los pokemon y aplica una funcion pero en orden alfabetico. En consecuencia,  me conviene usar un abb e insertar los pokemon segun su nombre, operacion cuyo orden de complejidad es O(log(n)) (suponiendo que el arbol esta balanceado, lo cual no siempre es cierto).
Luego, para el vector de entrenadores, lo reemplace directamente con una lista.  Esto es asi ya que desde el punto de vista del hospital, los entrenadores no tienen porque estar ubicados en una cola (como lo van a hacer en simulador). Ademas, hace que algunas operaciones, como el copiado de la lista, sean mas simples que usar una cola.
Asimismo, cambie algunos campos en las estructuras de pokemon y entrenador:
+ A los pokemon, les agregue un puntero a su entrenador. Es decir, ahora puedo saber cual es el entrenador de cada pokemon teniendo unicamente la estructura del pokemon. Este campo reemplaza al campo de id_entrenador que habia agregado en mi anterior implementacion.
+ Por el otro lado, a los entrenadores les agregue una lista de sus pokemon. Nuevamente, desde el punto de vista del entrenador, no tiene sentido tener un heap minimal de pokemon (de esto se encarga el simulador). Este campo tiene como fin optimizar la busqueda de pokemon de cada entrenador. A la hora de atender un entrenador en el simulador, no es necesario iterar por todos los pokemon del hospital, sino que, dado un entrenador, podemos saber quienes son sus pokemon con un orden de complejidad muy reducido.

En la implementacion del simulador, decidi agregar diversos campos. Muchos de ellos son campos que guardan estadisticas del simulador, como por ejemplo la cantidad de intentos a la hora de adivinar un nivel, los puntos, la cantidad de pokemon atendidos y la cantidad de eventos simulados. No obstante, tambien agregue una cola de entrenadores a atender, en la cual los entrenadores esperan a su turno para entregar sus pokemon. Ademas, implemente un heap minimal de pokemon, ordenado segun el nivel de cada uno. Esto se debe a que la complejidad de obtener el pokemon de nivel mas bajo
es O(log(n)). Ademas, agregue un puntero al pokemon siendo tratado actualmente. Finalmente, al momento de guardar las distintas dificultades, utilice una lista. En un principio, habia pensado usar un hash, cuya clave para cada elemento sea el nombre de la dificultad. Sin embargo, me encontre con el problema de que para obtener la dificultad, me pasan por parametro el id de la misma en vez del nombre. El id basicamente representa el indice en la lista donde se lo guardo, por lo que aunque el hash sea mas eficaz en terminos de busqueda, no lo puedo usar en esta ocasion ya que no dispongo de una clave para buscar elementos. Al igual que con el pokemon en tratamiento, tengo un puntero que indica la dificultad seleccionada. Una alternativa a esto es tener el id de la dificultad seleccionada, sin embargo, cada vez que quiero atender un pokemon, deberia recorrer la lista buscando este indice por lo cual no es del todo eficiente

1. Detalles de alguna función

   Algún detalle importante sobre alguna función.

2. Detalle de otra función

   Algún detalle de otra función.

#### 3. Diagramas

1. Diagrama1

   ![Diagrama1](https://i.imgur.com/KvYn8UD.png)

   Explicacion del diagrama1, en caso de ser necesaria.

2. Diagrama2

   ![Diagrama2](https://i.imgur.com/nhqXNr6.png)

   Explicacion del diagrama2, en caso de ser necesaria.