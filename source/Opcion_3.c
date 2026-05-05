#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include "Funciones_basicas.h"
#include "Matrices.h"
#include "Opciones.h"

void Obtener_puntos (FILE *archivo, Matriz *puntos, short int *error) {
	/*
	Lee los números desde el archivo CSV y los
	transforma en números en una matriz de n*2.

	Puede devolver dos números para el erros:
	1: Uno de los datos leídos no es un número.
	2: No se respetó el formato CSV para una tabla de
	   2 columnas.
	3: A uno de los puntos le hace falta una
	   coordenada.
	4: No se tiene la cantidad necesaria de puntos
	   para hacer el spline.
	5: Se tiene dos puntos que tienen misma coordenada x.
	*/

	int i;
	int caracteres = 1, numeros = 0, continuar;
	char *cadena, caracter, *fin_cadena, ultimo_separador = 0;
	double numero_leido, temporal;

	(*puntos).entrada = (double *) malloc (sizeof(double));
	cadena = (char *) malloc (sizeof(char));
	cadena[0] = 0;

	// Leyendo caracter por caracter.
	do {
		caracter = fgetc(archivo);
		cadena = (char *) realloc (cadena, sizeof(char) * (++caracteres));
		if (caracter == ',' || caracter == '\n') {
			if ((ultimo_separador == ',' && caracter == ',') || (ultimo_separador == '\n' && caracter == '\n')) {
				puts ("Error: No se respeto el formato de una tabla de 2 columnas.");
				*error = 2;
				free (cadena);
				if ((*puntos).entrada != NULL) free ((*puntos).entrada);
				return;
			}
			cadena[caracteres - 1] = 0;
			numero_leido = strtod (cadena, &fin_cadena);
			if (*fin_cadena != 0 && *fin_cadena != ' ' && *fin_cadena != ',' && *fin_cadena != '\n') {
				puts ("Error: Uno de los datos no es un número.");
				*error = 1;
				free(cadena);
				if ((*puntos).entrada != NULL) free ((*puntos).entrada);
				return;
			} else {
				ultimo_separador = caracter;
				(*puntos).entrada = (double *) realloc ((*puntos).entrada, sizeof(double) * (++numeros));
				(*puntos).entrada[numeros - 1] = numero_leido;
				caracteres = 1;
				free(cadena);
				cadena = (char *) malloc (sizeof(char));
				cadena[0] = 0;
			}
		} else {
			cadena[caracteres - 2] = caracter;
			cadena[caracteres - 1] = 0;
		}
	} while ( caracter != EOF);
	free (cadena);

	// Verificando que se tengan n*2 datos leídos.
	if (numeros % 2 != 0) {
		puts ("Error: A uno de los puntos le falta una coordenada.");
		free((*puntos).entrada);
		*error = 3;
		return;
	}

	(*puntos).filas = numeros / 2;
	(*puntos).columnas = 2;

	if ((*puntos).filas < 3) {
		puts ("Error: No se tiene la suficiente cantidad de puntos para hacer el spline cúbico natural. Se requiere de al menos 3 puntos.");
		free((*puntos).entrada);
		*error = 4;
		return;
	}

	// Ordenando los puntos
	do {
		continuar = 1;
		for (i = 1; i < (*puntos).filas; i++)
			if (MATRIZ_ENTRADA (*puntos, i-1, 1) > MATRIZ_ENTRADA (*puntos, i, 1)) {
				// Intercambiando x.
				temporal = MATRIZ_ENTRADA (*puntos, i-1, 1);
				MATRIZ_ENTRADA (*puntos, i, 1) = MATRIZ_ENTRADA (*puntos, i-1, 1);
				MATRIZ_ENTRADA (*puntos, i-1, 1) = temporal;
				// Intercambiando y.
				temporal = MATRIZ_ENTRADA (*puntos, i-1, 2);
				MATRIZ_ENTRADA (*puntos, i, 1) = MATRIZ_ENTRADA (*puntos, i-1, 2);
				MATRIZ_ENTRADA (*puntos, i-1, 2) = temporal;
				// Asegurando repetición hasta que todos los datos estén ordenados.
				continuar = 0;
			} else if (MATRIZ_ENTRADA (*puntos, i-1, 1) == MATRIZ_ENTRADA (*puntos, i, 1)) {
				puts ("Error: Se tiene dos o más puntos con la misma coordenada x.");
				free ((*puntos).entrada);
				*error = 5;
				return;
			}
	} while (continuar == 0);
}

int screen_x(double x, double xmin, double xmax, int window_width) {
	return (int)(((x - xmin) / (xmax - xmin)) * (double) window_width);
}

int screen_y(double y, double ymin, double ymax, int window_height) {
	return (int)(((ymax - y) / (ymax - ymin)) * (double) window_height);
}

double Evaluar (Matriz puntos, Matriz resultados, double x) {
	int i;
	double suma = 0.0;

	for (i = 0; i < puntos.filas - 1; i++)
		if (MATRIZ_ENTRADA (puntos, i, 1) <= x && x <= MATRIZ_ENTRADA (puntos, i+1, 1)) {
			suma += MATRIZ_ENTRADA (resultados, i, 1) * pow (x - MATRIZ_ENTRADA (puntos, i, 1), 3.0); // a (x - x_i)^3.
			suma += MATRIZ_ENTRADA (resultados, i, 2) * pow (x - MATRIZ_ENTRADA (puntos, i, 1), 2.0); // b (x - x_i)^2.
			suma += MATRIZ_ENTRADA (resultados, i, 3) * (x - MATRIZ_ENTRADA (puntos, i, 1)); // c (x - x_i).
			suma += MATRIZ_ENTRADA (resultados, i, 4); // d.
			return suma;
		}
	
	return suma;
}

double Maxima_y (Matriz puntos) {
	int i;
	double maximo = puntos.entrada[1];
	for (i = 1; i < puntos.filas; i++)
		if (maximo < MATRIZ_ENTRADA (puntos, i, 2))
			maximo = MATRIZ_ENTRADA (puntos, i, 2);
	return maximo;
}

void Dibujar_spline (Matriz puntos, Matriz resultados) {
	int graficando = 1;
	SDL_Event event;
	int window_width = 1000;
	int window_height = 1000;
	int screen_x1, screen_x2, screen_y1, screen_y2;
	double xmin = MATRIZ_ENTRADA (puntos, 1, 1);
	double xmax = MATRIZ_ENTRADA (puntos, puntos.filas, 1);
	double ymin = MATRIZ_ENTRADA (puntos, 1, 2);
	double ymax = Maxima_y (puntos);
	double step = ( xmax - xmin ) / ((double) window_width);
	double x, y1, y2;
	int thickness = (window_width < window_height ? window_height : window_width) / 100;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"Grafica del spline cúbico natural",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height,
		SDL_WINDOW_SHOWN
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Dibujando ejes
	// Eje X
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	thickLineRGBA(
		renderer,
		0,
		screen_y (0.0, ymin, ymax, window_height),
		window_width,
		screen_y (0.0, ymin, ymax, window_height),
		thickness,
		255,
		0,
		0,
		255);
	// Eje Y
	thickLineRGBA(
		renderer,
		screen_x (0.0, xmin, xmax, window_width),
		0,
		screen_x (0.0, xmin, xmax, window_width),
		window_height,
		thickness,
		255,
		0,
		0,
		255);

	// Dibujando el Spline
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (x = xmin; x < xmax; x += step) {
		y1 = Evaluar (puntos, resultados, x);
		y2 = Evaluar (puntos, resultados, x + step);

		screen_x1 = screen_x (x, xmin, xmax, window_width);
		screen_y1 = screen_y (y1, ymin, ymax, window_height);

		screen_x2 = screen_x (x + step, xmin, xmax, window_width);
		screen_y2 = screen_y (y2, ymin, ymax, window_height);

		thickLineRGBA(renderer, screen_x1, screen_y1, screen_x2, screen_y2, thickness, 255, 0, 0, 255);
	}

	SDL_RenderPresent(renderer);

	// Graficando hasta que la tecla espacio sea presionada.
	puts ("Presione la tecla ESPACIO dentro de la ventana con la gráfica para cerrarla.");
	while (graficando == 1) {
		while (SDL_PollEvent (&event)) {
			if (event.type == SDL_QUIT)
				graficando = 0;

			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.sym == SDLK_SPACE)
					graficando = 0;
		}
	}

	// Dejando de graficar.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Opcion_3 () {
	int i;
	int reintentar, respuesta;
	short int error;
	double temporal_1, temporal_2;
	char *nombre_del_archivo;
	Matriz puntos, diferencias, coeficientes, vector_independiente, solucion, resultados;
	FILE *csv;

	// Obteniendo el archivo y leyendo los datos de este.
	do {
		do {
			reintentar = 0;
			error = 0;
			printf ("Inserte la ruta del archivo con los puntos (formato en CSV): ");
			Leer_cadena (&nombre_del_archivo);

			// Verificando que exista el archivo.
			csv = fopen (nombre_del_archivo, "r");
			if (csv == NULL) {
				puts ("No se encontró un archivo con tal nombre.");
				reintentar = Pregunta_cerrada ("¿Quiere escribir de nuevo el nombre del archivo?");
				if (reintentar == 0) return;
			} else {
				Obtener_puntos (csv, &puntos, &error);
				fclose (csv);
				if (error != 0) {
					reintentar = Pregunta_cerrada ("¿Quiere intentar con otro archivo?");
					if (reintentar == 0) return;
				}
			}
		} while (reintentar == 1);

		// Calculando las h_i y las diferencias divididas.
		diferencias.filas = puntos.filas - 1;
		diferencias.columnas = 2;
		diferencias.entrada = (double *) malloc (sizeof(double) * diferencias.filas * diferencias.columnas);
		for (i = 1; i <= diferencias.filas; i++) {
			MATRIZ_ENTRADA (diferencias, i, 1) = MATRIZ_ENTRADA (puntos, i+1, 1) - MATRIZ_ENTRADA (puntos, i, 1);
			MATRIZ_ENTRADA (diferencias, i, 2) = (MATRIZ_ENTRADA (puntos, i+1, 2) - MATRIZ_ENTRADA (puntos, i, 2)) / MATRIZ_ENTRADA (diferencias, i, 1);
		}

		// Obteniendo la matriz de coeficientes h.
		coeficientes.filas = diferencias.filas - 1;
		coeficientes.columnas = coeficientes.filas;
		coeficientes.entrada = (double *) malloc (sizeof(double) * coeficientes.filas * coeficientes.columnas);
		// Inicializando la matriz en ceros.
		for (i = 0; i < coeficientes.filas * coeficientes.columnas; i++)
			coeficientes.entrada[i] = 0.0;
		// Inicializando números de la primera fila.
		MATRIZ_ENTRADA (coeficientes, 1, 1) = 2.0 * (MATRIZ_ENTRADA (diferencias, 1, 1) + MATRIZ_ENTRADA (diferencias, 2, 1));
		MATRIZ_ENTRADA (coeficientes, 1, 2) = MATRIZ_ENTRADA (diferencias, 2, 1);
		for (i = 2; i < coeficientes.filas; i++) {
			MATRIZ_ENTRADA (coeficientes, i, i-1) = MATRIZ_ENTRADA (diferencias, i, 1);
			MATRIZ_ENTRADA (coeficientes, i, i) = 2.0 * (MATRIZ_ENTRADA (diferencias, i, 1) + MATRIZ_ENTRADA (diferencias, i+1, 1));
			MATRIZ_ENTRADA (coeficientes, i, i+1) = MATRIZ_ENTRADA (diferencias, i+1, 1);
		}
		// Inicializando números de la última fila.
		MATRIZ_ENTRADA (coeficientes, coeficientes.filas, coeficientes.filas-1) = MATRIZ_ENTRADA (diferencias, coeficientes.filas, 1);
		MATRIZ_ENTRADA (coeficientes, coeficientes.filas, coeficientes.filas) = 2.0 * (MATRIZ_ENTRADA (diferencias, coeficientes.filas, 1) + MATRIZ_ENTRADA (diferencias, coeficientes.filas+1, 1));

		// Obteniendo el vector independiente.
		vector_independiente.filas = coeficientes.filas;
		vector_independiente.columnas = 1;
		vector_independiente.entrada = (double *) malloc (sizeof(double) * vector_independiente.filas * vector_independiente.columnas);
		for (i = 1; i <= vector_independiente.filas; i++)
			MATRIZ_ENTRADA (vector_independiente, i, 1) = 6.0 * (MATRIZ_ENTRADA (diferencias, i+1, 2) - MATRIZ_ENTRADA (diferencias, i, 2));
		
		// Obteniendo el vector solución.
		Invertir_matriz (coeficientes);
		solucion = Multiplicar_matrices (coeficientes, vector_independiente);
		// Añadiendo cero al principio y final del vector.
		solucion.filas+=2;
		solucion.entrada = realloc (solucion.entrada, sizeof(double) * solucion.filas * solucion.columnas);
		temporal_1 = solucion.entrada[0];
		solucion.entrada[0] = 0;
		for (i = 1; i < solucion.filas * solucion.columnas - 1; i++) {
			temporal_2 = solucion.entrada[i];
			solucion.entrada[i] = temporal_1;
			temporal_1 = temporal_2;
		}
		solucion.entrada[solucion.filas * solucion.columnas - 1] = 0;

		// Obteniendo los coeficientes de los polinomios.
		resultados.filas = diferencias.filas;
		resultados.columnas = 4;
		resultados.entrada = (double *) malloc (sizeof(double) * resultados.filas * resultados.columnas);
		for (i = 1; i <= resultados.filas; i++) {
			// Obteniendo coeficiente 'a'.
			MATRIZ_ENTRADA (resultados, i, 1) = (MATRIZ_ENTRADA (solucion, i+1, 1) - MATRIZ_ENTRADA (solucion, i, 1)) / (6.0 * MATRIZ_ENTRADA (diferencias, i, 1));
			// Obteniendo coeficiente 'b'.
			MATRIZ_ENTRADA (resultados, i, 2) = MATRIZ_ENTRADA (solucion, i, 1) / 2.0;
			// Obteniendo coeficiente 'c'.
			MATRIZ_ENTRADA (resultados, i, 3) = MATRIZ_ENTRADA (diferencias, i, 2) - (MATRIZ_ENTRADA (solucion, i+1, 1) + 2.0 * MATRIZ_ENTRADA (solucion, i, 1)) * MATRIZ_ENTRADA (diferencias, i, 1) / 6.0;
			// Obteniendo coeficiente 'd'.
			MATRIZ_ENTRADA (resultados, i, 4) = MATRIZ_ENTRADA (puntos, i, 2);
		}

		// Imprimiendo sistema que se resolvió.
		puts ("Sistema que se resolvió:");
		for (i = 1; i <= coeficientes.filas; i++)
			printf ("%.3lf S_%d + 2 (%.3lf + %.3lf) S_%d + %.3lf S_%d = 6 ( %.3lf - %.3lf)\n",
				MATRIZ_ENTRADA (diferencias, i, 1),
				i-1,
				MATRIZ_ENTRADA (diferencias, i, 1),
				MATRIZ_ENTRADA (diferencias, i+1, 1),
				i,
				MATRIZ_ENTRADA (diferencias, i+1, 1),
				i+1,
				MATRIZ_ENTRADA (diferencias, i+1, 2),
				MATRIZ_ENTRADA (diferencias, i, 2));
		puts("");
		
		// Imprimiendo tabla con las S_i, a_i, b_i, c_i y d_i.
		puts("Tabla con los coeficientes del polinomio:");
		puts ("| i | S_i | a_i | b_i | c_i | d_i |");
		for (i = 1; i <= resultados.filas; i++)
			printf ("| %3d | %6.3lf | %6.3lf | %6.3lf | %6.3lf | %6.3lf |\n",
				i,
				MATRIZ_ENTRADA (solucion, i, 1),
				MATRIZ_ENTRADA (resultados, i, 1),
				MATRIZ_ENTRADA (resultados, i, 2),
				MATRIZ_ENTRADA (resultados, i, 3),
				MATRIZ_ENTRADA (resultados, i, 4));
		printf("| %3d | %6.3lf | - | - | - | - |\n",
			i+1,
			MATRIZ_ENTRADA (solucion, i+1, 1));
		puts ("");

		// Dibujando el polinomio
		Dibujar_spline (puntos, resultados);

		// Preguntando por intentar con otro sistema.
		respuesta = Pregunta_cerrada ("¿Quiére probar con otro conjunto de puntos?");
	} while (respuesta == 1);
}
