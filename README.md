# Tarea Programada # 1  - Planificador del CPU

## Integrantes
### - Anner Calvo Granados 2017107433
### - Kenneth Corrales Lizano 2017151342 
### - Andres Cornejo Hidalgo 2017147856


## Indice
1. [Introduccion](#Introduccion)
2. [Estrategia de Solucion](#estrategia-de-solucion)
2. [Analisis de Resultados](#analisis-de-resultados)
3. [Lecciones Aprendidas](#lecciones-aprendidas)
4. [Casos de Prueba](#casos-de-prueba)
5. [Comparativa](#comparativa)
6. [Manual de Usuario](#manual-de-usuario)
7. [Bitacora](#bitacora)
8. [Bibliografia](#bibliografia)

## Introduccion

El sistema operativo dede de encargarse de mantener un uso eficiente de los recursos (memoria,CPU, procesos, I/O ,etc.), en este trabajo vamos a estudiar como el sistema operativo maneja los procesos especificamente, para esto se planea desarrollar un simulador del planificador del CPU el cual implementara los algoritmos de:

    - First in First Out
    - Shortest Job First
    - Highest Priority First
    - Round Robin




## Estrategia de Solucion
Para simular como se envian los procesos se plantea la creacion de un sistema cliente - servidor donde el cliente sera el que se encarga de enviar los procesos, los cuales poseen 2 datos: burst,prioridad por otro lado tenemos el servidor que fungira como el planificador y es en este donde se concentra la logica de los algoritmos mencionados anteriormente.

Existe un hilo que sera el job scheduler, el cual se encarga de recibir los procesos del cliente crearles un PCB y cargarlos a memoria para ser ejecutados, se tiene tambien el CPU Scheduler, este hilo sera el encargado de seleccionar los procesos en ready y procesarlos, basado en un algoritmo de planificacion.

Se creo la estructura PCB la cual es la que maneja la informacion del proceso , su estado, PID, burst prioridad ...

Ademas se utilizan sockets para realizar la conexion entre el cliente y el servidor, asi como la creacion de multihilos para el manejo de los sockets del servidor, esto es conveniente ya que se pueden conectar multiples clientes a un mismo servidor.

## Analisis de Resultados
- Cliente:
    
    - Modo manual: 100%
    - Modo automatico: 100%
    - Socket de envio de informacion : 100%
    - Recepcion de PID enviado por el servidor : 100%
    - Eliminacion de los hilos despues de recibido el PID : 100%

- Servidor:

    - Hilo Job Scheduler : 100%
    - Hilo CPU Scheduler : 100%
    - Creacion de PCB : 100%
    - Impresion del Context Switch: 100%
    - Impresion al terminar un proceso: 100%
    - Consultar la cola del ready : 100%
    - Finalizar simulacion y mostrar informacion : 100%
    - FIFO : 100%
    - SJF : 100%
    - HPF : 100%
    - RoundRobin : 100%

- Documentacion : 100%




## Lecciones Aprendidas

1. El manejo de los procesos es una tarea que requiere de mucho cuidado, y puede que un mayor gasto de los recursos sin embargo puede aprovechar de mejor manera los mismos.

2. La creacion de hilos en C es bastante sencillo, sin embargo un correcto manejo de estos, no lo es, ya que si los diferentes hilos comparten recursos, se pueden ocasionar fallos. 

3. Utilizar bibliotecas para el manejo de estructuras es mas conveniente que crearlas desde 0, para este trabajo se utilizo la biblioteca sys\queue.h del kernel de linux lo que nos facilito no tener que realizar las listas dinamicas desde 0 

4. El manejo de la memoria y el uso de punteros da muchas ventajas asi como desventajas, se puede reducir el uso de la memoria al utilizar y enviar la informacion por referencia en vez  de por copia, sin embargo, esto anade un nivel de complejidad mayor.

## Casos de Prueba

## Comparativa 

Los hilos que se utilizan tanto en JAVA como los POSIX se utiliza para ejecutar procesos en paralelo, mejorando con esto el rendimiento de los programas. Ahora bien existen ciertas caracteristicas que definen a cada tipo de hilo los cuales veremos a continuacion. Uno de los principales y de los mas obvios es el hecho de que los hilos POSIX son manejados directamente por el sistema operativo, en el caso de JAVA, estos son manejados por la maquina virtual (Java Virtual Machine), en terminos de la comunicacion, los hilos en C comparten informacion en comun lo que facilita que la comunicacion sea mas rapida, para su contricante en Java, utiliza metodos como notify() o notifyAll() para poder comunicarse entre hilos. La sincronizacion es otro factor importante en el lado de POSIX tenemos que se utilizan MUTEX para llevar a cabo la sincronizacion, sincronizacion por bloques y monitores es la manera comun de sincronizar hilos en Java. 



## Manual de Usuario
Este trabajo utiliza la biblioteca <pthread.h> de C/C++. Ademas, los proyectos tanto del cliente como del servidor estan creados utilizando Qt version 6.2.3. El compilador utilizado es GCC 11.

Para compilar la solucion se deben de seguir los siguientes pasos:

- Crear una terminal en la ruta donde se encuentra el archivo main.c
- Para compilarlo se usa los siguientes comandos: 

    - Cliente:
    >  gcc -o main main.c -lpthread

    - Servidor:
    > gcc -o main main.c connection.c connection.h jobsched.c jobsched.h pcb.c pcb.h scheduler.c scheduler.h -lpthread

- Una vez aplicado el comando, se creara un ejecutable. Dependiendo de la solucion sera necesario cambiar los argumentos:

  - Cliente
  
  En caso del modo manual el programa solicita tambien la ruta del archivo que contiene los procesos, el cual debera de ser un archivo en formato .txt
  >  ./main manual {directorio}

    En el caso del modo automatico el programa solicita los argumentos: 
    - minBurst: el valor minimo que el sistema puede generar los burst de los procesos
    - maxBurst: el valor maximo que el sistema puede generar los burst de los procesos
    - ratio: es el tiempo de espera entre la creacion de un proceso y el siguiente 

  >  ./main auto {minBurst} {maxBurst} {ratio}


## Bitacora
No se realizaron consultas con el profesor.

Para la realizacion del proyecto se tomo la primera semana para estudiar los requerimientos y estudiar las bibliotecas, asi como realizar pequenas pruebas
del uso de sockets e hilos en C.

Para la segunda semana se programo toda la funcionalidad del proyecto, para ver el progreso se pueden revisar los commits
que se encuentran el repositorio de git 

[Repositorio GitHub](https://github.com/koruss/PlanificadorCPU.git)


## Bibliografia 

- [Manejo de Listas](http://infnis.wikidot.com/list-from-sys-queue-h)
- [Manejo de Listas con macros](https://man7.org/linux/man-pages/man3/list.3.html)
- [POSIX hilos](https://randu.org/tutorials/threads/)
- [Eliminar hilos](https://stackoverflow.com/questions/7961029/how-can-i-kill-a-pthread-that-is-in-an-infinite-loop-from-outside-that-loop)
- [Round Robin](https://www.javatpoint.com/round-robin-program-in-c)
- [Shortest Job First](https://learnprogramo.com/shortest-job-first-programs-in-c/)
- [Cliente-Servidor](http://www.mario-konrad.ch/blog/programming/multithread/tutorial-04.html)
- [Cliente-Servidor](https://www.geeksforgeeks.org/socket-programming-cc/)