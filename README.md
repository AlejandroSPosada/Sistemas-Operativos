# Guia para la compilacion y ejecucion del programa:

## Definicion de procesos:

Para definir la cantidad de procesos y sus estados iniciales, en el archivo `procesos.txt` coloque cada proceso con su respectiva PID y con los valores iniciales de los registro AX, BX, y CX.
Tambien debe de ser definida el quantum del que dispondra este proceso

### Ejemplo:

```
PID=1 AX=3 BX=1 CX=3 Quantum=2
PID=2 AX=0 BX=0 CX=0 Quantum=3
PID=3 AX=5 BX=2 CX=7 Quantum=4
...
PID=n AX=# BX=# CX=# Quantum=#
```

Siendo n el numero total de procesos y # un numero entero cualquiera.

## Definicion de instrucciones:

Para definir las instrucciones de cada proceso, se debe crear un archivo `#.txt` (siendo # la PID del proceso) que contenga las instrucciones del respectivo proceso.
Tenga en cuenta que debera crear un archivo de este estilo para cada proceso creado en `procesos.txt`.

### Ejemplo:
En el archivo `3.txt`, que corresponderia a las instrucciones a ser ejecutadas por el proceso de PID 3:

```
MUL CX, 2
SUB CX, AX
ADD BX, CX
INC AX
JMP 6
INC AX
NOP
NOP
```

## Ejecucion:

Una vez definidos los procesos y sus respectivas instrucciones, abra la consola y dirigase a la ubicacion de la carpeta que contiene `procesos.txt` y los archivos con las instrucciones.
Una vez ahi debera ejecutar el comando `g++ Main.cpp -o programa`, seguido por el comando `./programa `. Una vez realizado esto, el programa iniciara su ejecucion.
