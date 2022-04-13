# Tarea Programada # 1  - Planificador del CPU

## Integrantes
### - Anner Calvo Granados 2017107433
### - Kenneth Corrales Lizano 2017151342 
### - Andrés Cornejo Hidalgo 2017147856


## Índice
- [Tarea Programada # 1  - Planificador del CPU](#tarea-programada--1----planificador-del-cpu)
	- [Integrantes](#integrantes)
		- [- Anner Calvo Granados 2017107433](#--anner-calvo-granados-2017107433)
		- [- Kenneth Corrales Lizano 2017151342](#--kenneth-corrales-lizano-2017151342)
		- [- Andrés Cornejo Hidalgo 2017147856](#--andrés-cornejo-hidalgo-2017147856)
	- [Índice](#índice)
	- [Introducción](#introducción)
	- [Estrategia de Solución](#estrategia-de-solución)
	- [Análisis de Resultados](#análisis-de-resultados)
	- [Lecciones Aprendidas](#lecciones-aprendidas)
	- [Casos de Prueba](#casos-de-prueba)
	- [Comparativa](#comparativa)
	- [Manual de Usuario](#manual-de-usuario)
	- [Bitácora](#bitácora)
	- [Bibliografía](#bibliografía)

## Introducción

El sistema operativo debe de encargarse de mantener un uso eficiente de los recursos (memoria, CPU, procesos, I/O, etc.), en este trabajo vamos a estudiar como el sistema operativo maneja los procesos específicamente, para esto se planea desarrollar un simulador del planificador del CPU el cual implementara los algoritmos de:

    - First in First Out
    - Shortest Job First
    - Highest Priority First
    - Round Robin




## Estrategia de Solución
Para simular como se envían los procesos se plantea la creación de un sistema cliente-servidor donde el cliente será el que se encarga de enviar los procesos, los cuales poseen 2 datos: burst y prioridad. Por otro lado, tenemos el servidor que fungirá como el planificador y es en este donde se concentra la lógica de los algoritmos mencionados anteriormente.

Existe un hilo que será el job scheduler, el cual se encarga de recibir los procesos del cliente, crearles un PCB y cargarlos a memoria para ser ejecutados, se tiene también la CPU Scheduler, este hilo será el encargado de seleccionar los procesos en ready y procesarlos, basado en un algoritmo de planificación.

Se generó la estructura PCB la cual es la que maneja la información del proceso, su estado, PID, burst, prioridad ...

Además, se utilizan sockets para realizar la conexión entre el cliente y el servidor, así como la creación de multihilos para el manejo de los sockets del servidor, esto es conveniente, ya que se pueden conectar múltiples clientes a un mismo servidor.

## Análisis de Resultados
- Cliente:
    
    - Modo manual: 100%
    - Modo automático: 100%
    - Socket de envío de información: 100%
    - Recepción de PID enviado por el servidor: 100%
    - Eliminación de los hilos después de recibido el PID: 100%

- Servidor:

    - Hilo Job Scheduler: 100%
    - Hilo CPU Scheduler: 100%
    - Creación de PCB: 100%
    - Impresión del Context Switch: 100%
    - Impresión al terminar un proceso: 100%
    - Consultar la cola del ready: 100%
    - Finalizar simulación y mostrar información: 100%
    - FIFO: 100%
    - SJF: 100%
    - HPF: 100%
    - RoundRobin: 100%

- Documentación: 100%




## Lecciones Aprendidas

1. El manejo de los procesos es una tarea que requiere de mucho cuidado, y puede que un mayor gasto de los recursos, sin embargo, puede aprovechar de mejor manera los mismos.

2. La creación de hilos en C es bastante sencillo; sin embargo, un correcto manejo de estos, no lo es, ya que si los diferentes hilos comparten recursos, se pueden ocasionar fallos. 

3. Utilizar bibliotecas para el manejo de estructuras es más conveniente que crearlas desde 0, para este trabajo se utilizó la biblioteca sys\queue.h del kernel de Linux, lo que nos facilitó no tener que realizar las listas dinámicas desde 0 

4. El manejo de la memoria y el uso de punteros da muchas ventajas así como desventajas, se puede reducir el uso de la memoria al utilizar y enviar la información por referencia en vez  de por copia, sin embargo, esto añade un nivel de complejidad mayor.

## Casos de Prueba

## Comparativa 

Los hilos que se utilizan tanto en JAVA como los POSIX se utiliza para ejecutar procesos en paralelo, mejorando con esto el rendimiento de los programas. Ahora bien, existen ciertas características que definen a cada tipo de hilo los cuales veremos a continuación. Uno de los principales y de los más obvios es el hecho de que los hilos POSIX son manejados directamente por el sistema operativo, en el caso de JAVA, estos son manejados por la máquina virtual (Java Virtual Machine), en términos de la comunicación, los hilos en C comparten información en común lo que facilita que la comunicación sea más rápida, para su contrincante en Java, utiliza métodos como notify() o notifyAll() para poder comunicarse entre hilos. La sincronización es otro factor importante en el lado de POSIX tenemos que se utilizan MUTEX para llevar a cabo la sincronización, sincronización por bloques y monitores es la manera común de sincronizar hilos en Java. 



## Manual de Usuario
Este trabajo utiliza la biblioteca <pthread.h> de C/C++. Además, los proyectos tanto del cliente como del servidor están creados utilizando Qt versión 6.2.3. El compilador utilizado es GCC 11.

Para compilar la solución se deben de seguir los siguientes pasos:

- Crear una terminal en la ruta donde se encuentra el archivo main.c
- Para compilarlo se usa los siguientes comandos: 

    - Cliente:
    >  gcc -o main main.c -lpthread

    - Servidor:
    > gcc -o main main.c connection.c connection.h jobsched.c jobsched.h pcb.c pcb.h scheduler.c scheduler.h -lpthread

- Una vez aplicado el comando, se producirá un ejecutable. Dependiendo de la solución será necesario cambiar los argumentos:

  - Cliente
  
  En caso del modo manual, el programa solicita también la ruta del archivo que contiene los procesos, el cual deberá de ser un archivo en formato .txt
  >  ./main manual {directorio}

    En el caso del modo automático, el programa solicita los argumentos: 
    - minBurst: el valor mínimo que el sistema puede generar los burst de los procesos
    - maxBurst: el valor máximo que el sistema puede generar los burst de los procesos
    - ratio: es el tiempo de espera entre la creación de un proceso y el siguiente 

  >  ./main auto {minBurst} {maxBurst} {ratio}


## Bitácora
No se realizaron consultas con el profesor.

Para la realización del proyecto se tomó la primera semana para estudiar los requerimientos y estudiar las bibliotecas, así como realizar pequeñas pruebas
del uso de sockets e hilos en C.

Para la segunda semana se programó toda la funcionalidad del proyecto, para ver el progreso se pueden revisar los commits
que se encuentran el repositorio de git 

[Repositorio GitHub](https://github.com/koruss/PlanificadorCPU.git)


## Bibliografía 

- [Manejo de Listas](http://infnis.wikidot.com/list-from-sys-queue-h)
- [Manejo de Listas con macros](https://man7.org/linux/man-pages/man3/list.3.html)
- [POSIX hilos](https://randu.org/tutorials/threads/)
- [Eliminar hilos](https://stackoverflow.com/questions/7961029/how-can-i-kill-a-pthread-that-is-in-an-infinite-loop-from-outside-that-loop)
- [Round Robin](https://www.javatpoint.com/round-robin-program-in-c)
- [Shortest Job First](https://learnprogramo.com/shortest-job-first-programs-in-c/)
- [Cliente-Servidor](http://www.mario-konrad.ch/blog/programming/multithread/tutorial-04.html)
- [Cliente-Servidor](https://www.geeksforgeeks.org/socket-programming-cc/)


