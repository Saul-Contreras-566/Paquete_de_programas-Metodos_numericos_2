#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Funciones_basicas.h"
#include "Opciones.h"





double redondear(double numero, int digitos) {
	double potencia = pow(10.0, (double) digitos);
	return floor(numero * potencia) / potencia;
}

double funcion1(double x) {
	return pow(x, 4) * (sqrt(3.0 + 2.0 * pow(x, 2)) / 3.0);
}

double funcion2(double x) {
	return pow(x, 5) / pow(pow(x, 2) + 4.0, 1.0 / 5.0);
}

// Método de Romberg
void romberg(double (*f)(double), double a, double b, int digitos, int intervalos) {
	double x, error, suma;
	double h = (b - a) / (double) intervalos;
	double *extrapolaciones = malloc (sizeof(double));
	double temporal_1, temporal_2;
	int i, n; // Variables para iteraciones.

	// Primeros trapecios.
	x = a + h;
	suma = 0.0;
	while (x < b) {
		suma += f(x);
		x += h;
	}
	extrapolaciones[0] = h * (f(a) + f(b) + 2.0 * suma) / 2.0;

	n = 1;
	do {
		// Realizando la siguiente iteración de trapecios.
		h /= 2.0;
		x = a + h;
		suma = 0.0;
		while (x < b) {
			suma += f(x);
			x += 2.0 * h;
		}

		// Realizando siguiente extrapolación.
		extrapolaciones = (double *) realloc (extrapolaciones, sizeof(double) * (++n));
		temporal_1 = extrapolaciones[0] / 2.0 + h * suma;
		temporal_2 = temporal_1 + (temporal_1 - extrapolaciones[0]) / 3.0;
		extrapolaciones[0] = temporal_1;
		for (i = 1; i < n-1; i++) {
			temporal_1 = temporal_2;
			temporal_2 = temporal_1 + (temporal_1 - extrapolaciones[i]) / (pow(4.0, (double) (i+1)) - 1);
			extrapolaciones[i] = temporal_1;
		}
		extrapolaciones[n-1] = temporal_2;
		error = fabs(redondear(extrapolaciones[n-1], digitos) - redondear(extrapolaciones[n-2], digitos));
	} while (error > 0.0);

	puts ("");
	printf ("Mejor aproximación: %.10lf.\n", extrapolaciones[n-1]);
	printf ("Orden del error: O(h^%d).\n", (n-1) * 2);
	free(extrapolaciones);
}

void Opcion_4() {
	int opcion, digitos, respuesta, intervalos;
	double a, b;
	
	do {
		LIMPIAR_PANTALLA();
		puts("Seleccione una opción:");
		puts("0. Salir");
		puts("1. Integrar f(x) = x^4 * (sqrt(3 + 2x^2) / 3)");
		puts("2. Integrar f(x) = x^5 / (x^2 + 4)^(1/5)");
		puts("Seleccionar:");
		opcion = Leer_entero_entre(0,2);

		if(opcion != 0) {
			puts("\nIngrese el limite inferior de integracion (a):");
			a = Leer_real();
			puts("Ingrese el limite superior de integracion (b):");
			b = Leer_real_que_sea(">", a);
			puts("Inserte el número de digitos decimales de precisión (un entero entre 1 y 10):");
			digitos = Leer_entero_entre (1, 10);
			puts("Inserte el número de intervalos a usar en la regla de trapecios:");
			intervalos = Leer_entero_que_sea (">", 0);
			
			switch (opcion) {
				case 1: romberg(funcion1, a, b, digitos, intervalos); break;
				case 2: romberg(funcion2, a, b, digitos, intervalos); break;
			}
		
			respuesta = Pregunta_cerrada("¿Quiére insertar otros datos?");
		}

	} while (opcion != 0 && respuesta == 1); 
} 
