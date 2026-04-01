#include "Funciones_basicas.h"
#include "Matrices.h"
#include "Opciones.h"

// Macro para acceder a las entradas de la tabla.
#define VALOR(tabla, x, f) (*tabla).entrada[(x) * (*tabla).columnas + f]
/*
`tabla' : es el apuntador de la tabla.
`x'     : indica el valor de la x_i a usar.
`f'     : tomando solo los valores de 0 o 1, indica si usar
          el valor de x_i o el de f(x_i) respectivamente.
*/

void Corregir_entradas_de_tabla (double *entrada, int filas, int columnas) {
	int i; // Variable para iteraciones
	int x, y; // Variables para la selección de la entrada a corregir
	int respuesta; // Respuesta del usuaior para corregir otra entrada o no

	do {
		// Mostrando tabla
		puts("| i   | x_i      | f(x_i)   |");
		for (i = 0; i < filas; i++) {
			printf ("| %3d | %6.6lf | %6.6lf |\n", i, entrada[i * columnas], entrada[i * columnas + 1]);
		}

		// Preguntando por la corrección
		respuesta = Pregunta_cerrada ("¿Quiére corregir alguna entrada?");

		// Corrigiendo en caso de quererlo
		if (respuesta == 1) {
			// Pidiendo ubicación
			puts ("¿Cuál es la fila (valor de i) de la entrada?");
			x = Leer_entero_entre (0, filas - 1);
			puts ("¿Cuál es la columna de la entrada?");
			puts ("1. Columna de las x_i.");
			puts ("2. Columna de las f(x_i).");
			y = Leer_entero_entre (1, columnas);

			// Actualizando
			puts ("Inserte el nuevo número:");
			entrada[x * columnas + y - 1] = Leer_real ();
		}
	}
	while (respuesta == 1);
}
	

double *Leer_entradas_de_tabla_de_puntos (int filas, int columnas) {
	int i; // Variable para iteraciones
	double *entrada = malloc (sizeof (double) * filas * columnas); // Reservando memoria dinámica para las entradas

	// Leyendo las entradas
	for (i = 0; i < filas; i++) {
		printf("Valor de x_%d:\n", i);
		entrada[i * columnas] = Leer_real();
		printf("Valor de f(x_%d):\n", i);
		entrada[i * columnas + 1] = Leer_real();
	}
	
	// Corrigiendo entradas
	Corregir_entradas_de_tabla (entrada, filas, columnas);

	return entrada;
}

void Leer_tabla(Matriz *tabla) {
	int continuar;         // Usado para detenerse en caso de no cumplir algunas condiciones.
	double equidistancia;  // Usado para almacenar la distancia entre x_1 y x_0.
	double temp;           // Varible para realizar intercambios
	int i;                 // Variable para iteraciones.

	do {
		// Leyendo el número de puntos a leer.
		puts("¿Cuántos puntos quiere leer? (inserte un número mayor o igual que 2)");
		(*tabla).filas = Leer_entero_que_sea(">=", 2);

		// Leyendo las x_i y f(x_i).
		puts("Inserte los datos de la tabla.");
		(*tabla).entrada = Leer_entradas_de_tabla_de_puntos((*tabla).filas, (*tabla).columnas);

		do {
			// Ordenando los datos en base a las x_i de menor a mayor usando el método de la burbuja.
			do {
				continuar = 1;
				for (i=1; i<(*tabla).filas; i++)
					if (VALOR(tabla, i, 0) < VALOR(tabla, i-1, 0)) {
						// Intercambiando x_i y x_{i-1}.
						temp = VALOR(tabla, i, 0);
						VALOR(tabla, i, 0) = VALOR(tabla, i-1, 0);
						VALOR(tabla, i-1, 0) = temp;
						// Intercambiando f(x_i) y f(x_{i-1}).
						temp = VALOR(tabla, i, 1);
						VALOR(tabla, i, 1) = VALOR(tabla, i-1, 1);
						VALOR(tabla, i-1, 1) = temp;
						continuar = 0; // Esto para repetir las ordenaciones en caso de haber un intercambio.
					}
			} while (continuar == 0);

			// Verificando la equidistancia de los datos.
			continuar = 1;
			equidistancia = Redondear(VALOR(tabla, 1, 0) - VALOR(tabla, 0, 0), 10);
			for (i=2; i<(*tabla).filas; i++)
				if (Redondear(VALOR(tabla, i, 0) - VALOR(tabla, i-1, 0), 10) != equidistancia) {
					continuar = 0;
					puts("ADVERTENCIA: No se cumple la equidistancia de las x_i.");
					Corregir_entradas_de_tabla((*tabla).entrada, (*tabla).filas, (*tabla).columnas);
					break;
				}
		} while (continuar == 0);

	} while (continuar == 0);
}

void Diferenciador(Matriz tabla, Matriz *diferencias);
void Sumador(Matriz tabla, double punto, int grado, Matriz diferencias);

void Opcion_2() {
	Matriz  tabla;        // Aquí se almacenan los puntos $(x_i, f(x_i))$.
	Matriz  diferencias;  // Aquí se almacenan las diferencias.
	double  punto;        // La $x_i$ a interpolar.
	int     grado;        // El grado del polinomio.
	
	// Defininedo el número de columnas de la tabla
	tabla.columnas = 2;

	do {
		Leer_tabla(&tabla);

		Imprimir_matriz(tabla);

		do {
			puts("¿Cuál es el punto a interpolar?");
			punto = Leer_real_entre(tabla.entrada[0], tabla.entrada[(tabla.filas - 1) * tabla.columnas]);

			puts("¿Cuál es el grado del polinomio a interpolar?");
			grado = Leer_entero_entre(1, tabla.entrada[(tabla.filas - 1) * tabla.columnas]);

			Diferenciador(tabla, &diferencias);
			Sumador(tabla, punto, grado, diferencias);
			if (diferencias != NULL) free(diferencias.entrada); // Liberando memoria

		} while(Pregunta_cerrada("¿Quiere interpolar otro punto?") == 1);

		// Liberando memoria
		if (tabla.entrada != NULL) free(tabla.entrada);

	}
	while(Pregunta_cerrada("¿Quiere interpolar con otros datos?") == 1);
}
