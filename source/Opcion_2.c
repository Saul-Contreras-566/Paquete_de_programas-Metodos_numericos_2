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

void Diferenciador(Matriz tabla, Matriz *diferencias) {
    int i, j;
    int n = tabla.filas;


    diferencias->filas = n;
    diferencias->columnas = n;
    diferencias->entrada = (double *)calloc(n * n, sizeof(double));

    // La primera columna de la tabla de diferencias son los valores f(x_i)
    for (i = 0; i < n; i++) {
        diferencias->entrada[i * n] = VALOR(&tabla, i, 1);
    }

    // (progresivas)
    for (j = 1; j < n; j++) {
        for (i = 0; i < n - j; i++) {
            // Delta^j f_i = Delta^{j-1} f_{i+1} - Delta^{j-1} f_i
            diferencias->entrada[i * n + j] = diferencias->entrada[(i + 1) * n + (j - 1)] - diferencias->entrada[i * n + (j - 1)];
        }
    }

    // Imprime tabla de diferencias
    puts("\n--- Tabla de Diferencias Finitas ---");
    printf("%-5s | %-12s", "i", "f(x_i)");
    for(i = 1; i < n; i++) printf(" | D^%-2d f    ", i);
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%-5d | %-12.6lf", i, diferencias->entrada[i * n]);
        for (j = 1; j < n - i; j++) {
            printf(" | %-12.6lf", diferencias->entrada[i * n + j]);
        }
        printf("\n");
    }
}

void Sumador(Matriz tabla, double punto, int grado, Matriz diferencias) {
    int n = tabla.filas;
    double h = VALOR(&tabla, 1, 0) - VALOR(&tabla, 0, 0); // Cálculo de la distancia equidistante
    double resultado = 0.0, u, termino = 1.0;
    int i;

    // el grado no puede superar los datos disponibles
    if (grado >= n) grado = n - 1;

    // decidir entre Progresivo o Regresivo (punto medio de la tabla)
    double punto_medio = (VALOR(&tabla, 0, 0) + VALOR(&tabla, n - 1, 0)) / 2.0;

    if (punto <= punto_medio) {
        puts("\n[*] Usando Newton Progresivo.");
        u = (punto - VALOR(&tabla, 0, 0)) / h;
        resultado = diferencias.entrada[0]; // f(x_0)

        for (i = 1; i <= grado; i++) {
            termino *= (u - (i - 1)) / i;
            resultado += termino * diferencias.entrada[0 * diferencias.columnas + i];
        }
    } else {
        puts("\n[*] Usando Newton Regresivo.");
        u = (punto - VALOR(&tabla, n - 1, 0)) / h;
        resultado = diferencias.entrada[(n - 1) * diferencias.columnas]; // f(x_n)

        for (i = 1; i <= grado; i++) {
            termino *= (u + (i - 1)) / i;
            // Acceso a la diagonal inferior para diferencias hacia atrás
            resultado += termino * diferencias.entrada[(n - 1 - i) * diferencias.columnas + i];
        }
    }

    printf("\n>>> El valor interpolado es: %6.8lf\n", resultado);
}

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
			grado = Leer_entero_entre(1, tabla.filas - 1);

			Diferenciador(tabla, &diferencias);
			Sumador(tabla, punto, grado, diferencias);
			if (diferencias.entrada != NULL) free(diferencias.entrada); // Liberando memoria

		} while(Pregunta_cerrada("¿Quiere interpolar otro punto?") == 1);

		// Liberando memoria
		if (tabla.entrada != NULL) free(tabla.entrada);

	}
	while(Pregunta_cerrada("¿Quiere interpolar con otros datos?") == 1);
}
