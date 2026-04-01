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

void Lector(Matriz *tabla, int *salir) {
	int continuar;         // Usado para detenerse en caso de no cumplir algunas condiciones.
	double equidistancia;  // Usado para almacenar la distancia entre x_1 y x_0.
	double temp;           // Varible para realizar intercambios
	int i;                 // Variable para iteraciones.

	do {
		// Leyendo el número de puntos a leer.
		puts("¿Cuántos puntos quiere leer? (inserte un número mayor o igual que 2)");
		(*tabla).filas = Leer_entero_que_sea(">=", 2);

		// Leyendo las x_i y f(x_i).
		(*tabla).entrada = (double *) malloc((*tabla).filas * (*tabla).columnas * sizeof(double));
		for (i=0; i<(*tabla).filas; i++) {
			printf("Inserte el valor de la x_%d:\n", i);
			VALOR(tabla, i, 0) = Leer_real();
			printf("Inserte el valor de la f(x_%d):\n", i);
			VALOR(tabla, i, 1) = Leer_real();
		}

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

		// Verificando equidistancia de los datos.
		equidistancia = Redondear(VALOR(tabla, 1, 0) - VALOR(tabla, 0, 0), 10);
		for (i=2; i<(*tabla).filas; i++)
			if (Redondear(VALOR(tabla, i, 0) - VALOR(tabla, i-1, 0), 10) != equidistancia) {
				continuar = 0;
				break;
			}

		// En caso de que no se cumpla la equidistancia.
		if (continuar == 0) {
			puts("No se cumple la equidistancia en las x_i.");
			continuar = Pregunta_cerrada("¿Quiere leer otros datos?");
			if (continuar == 0)
				*salir = 1;
		}

	} while (continuar == 1);
}

void Diferenciador(Matriz *tabla, double *punto, int *grado, Matriz *diferencias);
void Sumador(Matriz *tabla, double *punto, int *grado, Matriz *diferencias);

void Opcion_2() {
	Matriz  tabla;        // Aquí se almacenan los puntos $(x_i, f(x_i))$.
	Matriz  diferencias;  // Aquí se almacenan las diferencias.
	double  punto;        // La $x_i$ a interpolar.
	int     grado;        // El grado del polinomio.
	int     salir;        // Para salir en caso de que no se quiera leer datos.
	
	// Defininedo el número de columnas de la tabla
	tabla.columnas = 2;

	do {
		Lector(&tabla, &salir);
		if (salir == 1) {
			free(tabla.entrada);
			return;
		}

		do {
			puts("¿Cuál es el punto a interpolar?");
			punto = Leer_real_entre(tabla.entrada[0], tabla.entrada[(tabla.filas - 1) * tabla.columnas]);

			puts("¿Cuál es el grado del polinomio a interpolar?");
			grado = Leer_entero_entre(1, tabla.entrada[(tabla.filas - 1) * tabla.columnas]);

			Diferenciador(tabla, punto, grado, &diferencias);
			Sumador(tabla, punto, grado, diferencias);
			if (diferencias != NULL) free(diferencias.entrada); // Liberando memoria

		} while(Pregunta_cerrada("¿Quiere interpolar otro punto?") == 1);

		// Liberando memoria
		if (tabla.entrada != NULL) free(tabla.entrada);

	}
	while(Pregunta_cerrada("¿Quiere interpolar con otros datos?") == 1);
}
