# Paquete de programas - Métodos numéricos II

![Portada](Portada.png)



## Integrantes del equipo

Del grupo 2403 de Métodos numéricos II:

- Carmona Maldonado Gibrán Isaí.
- Contreras Rojas Emanuel Saúl.
- Guzmán Ramos Carlos Emilio.



## Contenidos del programa

El programa puede

1. Resolver algunos sistemas de ecuaciones usando el método de Broyden.
2. Interpolar un punto por medio del polinomio de diferencias divididas de Newton.
3. Hacer spline cúbico natural de un conjunto puntos.
4. Calcular la integral de dos funciones usando la regla de trapecios y la extrapolación de Romberg.



## Compilación del programa

Para compilar el programa, necesita de las bibliotecas `SDL2` y `SDL2_gfx` junto con el compilador `gcc`. Si cuenta con GNU Make, puede usar el comando `make` en la carpeta raíz para compilar, de no contar con este, puede usar el siguiente comando:

```
gcc -Wall -Werror -Wextra -Iheaders source/Funciones_basicas.c source/Matrices.c source/Opcion_1.c source/Opcion_2.c source/Opcion_3.c source/main.c -o Paquete_de_programas-Métodos_numéricos.exe -lSDL2main -lSDL2_gfx -lSDL2 -lm
```
