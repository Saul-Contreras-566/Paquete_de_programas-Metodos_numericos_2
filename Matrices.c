/*
Código de las funciones con matrices.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int filas;
	int columnas;
	double *entrada;
} Matriz;

// FUNCIONES BÁSICAS CON MATRICES ------------------------------------------------------------------

double *Leer_entradas_de_matriz (int filas, int columnas) {
	/* Esta función está pensada para que se puedan leer las entradas de las matrices de una
	manera más flexible, por ejemplo, en un sistema de ecuaciones de la forma `Ax=b`, se puede
	pedir al usuario primero las dimensiones de las matriz `A` y luego simplemente copiar el
	número de filas al vector `b`, de modo que no se le pide o permite al usuario insertar dos
	veces el número de filas, impidiéndole así que inserte un sistema incoherente. */

	int i, j; // Variables para iteraciones
	int x, y; // Variables para la selección de la entrada a corregir
	int respuesta; // Respuesta del usuaior para corregir otra entrada o no
	double *entrada = malloc (sizeof (double) * filas * columnas); // Reservando memoria dinámica para las entradas

	// Leyendo las entradas
	for (i = 0; i < filas; i++)
		for (j = 0; j < columnas; j++) {
			printf ("Entrada (%d, %d) = ", i + 1, j + 1);
			entrada[i * columnas + j] = Leer_real ();
		}
	
	// Corrigiendo entradas
	do {
		// Mostrando matriz
		for (i = 0; i < filas; i++) {
			printf ("| ");
			for (j = 0; j < columnas; j++)
				printf ("%lf |", entrada[i * columnas + j]);
			printf ("\n");
		}

		// Preguntando por la corrección
		respuesta = Pregunta_cerrada ("¿Quiére corregir alguna entrada?");

		// Corrigiendo en caso de quererlo
		if (respuesta == 1) {
			// Pidiendo ubicación
			puts ("¿Cuál es la fila de la entrada?");
			x = Leer_entero_entre (1, filas);
			puts ("¿Cuál es la columna de la entrada?");
			y = Leer_entero_entre (1, columnas);

			// Actualizando
			puts ("Inserte el nuevo número:");
			entrada[(x - 1) * columnas + y - 1] = Leer_real ();
		}
	}
	while (respuesta == 1);
	
	return entrada;
}



Matriz Leer_matriz () {
	/* Esta función pide las dimensiones de la matriz y luego lee sus entradas. Las entradas
	son leídas por una función aparte. */

	Matriz lectura;

	// Pidiendo las dimensiones de la matriz
	puts ("¿Cuántas filas tiene la matriz?");
	lectura.filas = Leer_entero_que_sea (">=", 1);
	puts ("¿Cuántas columnas tiene la matriz?");
	lectura.columnas = Leer_entero_que_sea (">=", 1);

	// Leyendo entradas
	lectura.entrada = Leer_entradas_de_matriz (lectura.filas, lectura.columnas);

	// Devolviendo la matriz
	return lectura;
}



double Determinante (Matriz matriz) {
    /* Calcula el determinante de una matriz cuadrada usando eliminación gaussiana */
    
    if (matriz.filas != matriz.columnas) {
        printf ("ERROR: La matriz debe ser cuadrada para calcular el determinante.\n");
        return 0.0;
    }
    
    double determinante = 1.0;
    int i, j, k; // Variables para iteraciones
    int columna_del_maximo;
    double valor_temporal, factor;
    
    // Crear una copia de la matriz para no modificar la original
    Matriz temporal;
    temporal.filas = matriz.filas;
    temporal.columnas = matriz.filas;
    temporal.entrada = (double *) malloc (sizeof (double) * matriz.filas * matriz.filas);
    
    // Copiar los valores de la matriz original
    for (i = 0; i < matriz.filas * matriz.filas; i++) {
        temporal.entrada[i] = matriz.entrada[i];
    }
    
    // Aplicar eliminación gaussiana
    for (i = 0; i < matriz.filas; i++) {
        // Buscar el pivote máximo en la columna actual
        columna_del_maximo = i; // Seleccionando la columna del elmento de la diagonal principal para compararlo con el resto de elementos de su misma fila
        for (k = i + 1; k < matriz.filas; k++)
            if (fabs (temporal.entrada[k * matriz.columnas + i]) > fabs (temporal.entrada[columna_del_maximo * matriz.columnas + i]))
                columna_del_maximo = k;
        
        // Si el pivote es cero, el determinante es cero
        if (fabs (temporal.entrada[columna_del_maximo * matriz.columnas + i]) == 0.0) {
            free (temporal.entrada);
            return 0.0;
        }
        
        // Intercambiar filas si es necesario
        if (columna_del_maximo != i) {
            for (k = 0; k < matriz.filas; k++) {
                valor_temporal = temporal.entrada[i * matriz.columnas + k];
                temporal.entrada[i * matriz.columnas + k] = temporal.entrada[columna_del_maximo * matriz.columnas + k];
                temporal.entrada[columna_del_maximo * matriz.columnas + k] = valor_temporal;
            }
            determinante *= -1.0; // El determinante cambia de signo al intercambiar filas
        }
        
        // Hacer ceros debajo del pivote
        for (k = i + 1; k < matriz.filas; k++) {
            factor = temporal.entrada[k * matriz.columnas + i] / temporal.entrada[i * matriz.columnas + i];
            for (j = i; j < matriz.filas; j++) {
                temporal.entrada[k * matriz.columnas + j] -= factor * temporal.entrada[i * matriz.columnas + j];
            }
        }
        
        // Multiplicar el determinante por el pivote
        determinante *= temporal.entrada[i * matriz.columnas + i];
    }
    
    free (temporal.entrada);
    return determinante;
}



int Verificar_EDD (Matriz matriz) {
	/* Función que verifica que una función sea estrictamente dominante diagonalmente. Compara
	el valor absoluto de cada elemento de la diagonal con la suma de los valores absolutos del
	resto de elemtos de las misma fila que el del elemento en la diagonal. Devuelve `1` en caso
	de ser estrcitamente dominante diagonalmente o `0` en caso de no serlo. */

	int i, j; // Variables para iteraciones
	double suma; // Para verificar la dominancia
	int edd = 1; // Para indicar si la matriz es EDD o no, suponemos que lo es con 1 para después tratar de desmentirlo y, en caso de no ser EDD, detener el proceso de verificación
	
	// Verificando
	for (i = 0; i < matriz.filas && edd == 1; i++) {
		suma = 0.0;
		for (j = 0; j < matriz.columnas; j++)
			suma += (i == j) ? 0 : fabs (matriz.entrada[i * matriz.columnas + j]);
		edd = (matriz.entrada[i * (matriz.columnas + 1)] > suma) ? 1 : 0;
	}

	// Regresando el resultado de la verificación
	return edd;
}



double Norma_espectral (int numero_entradas, double *entrada) {
	/* Calcula la norma espectral de un vector. Un simple bucle `for` que itera comparaciones
	entre entradas del vector y en cada una de ellas se queda con la entrada más grande. De esta
	forma, obtiene la entrada de mayor magnitud y, por lo tanto, la norma espectral. */

	int i; // Variable para iteraciones
	double maximo;

	// Comparando
	maximo = fabs (entrada[0]);
	for (i = 1; i < numero_entradas; i++)
		maximo = (maximo < fabs (entrada[i])) ? fabs (entrada[i]) : maximo;
	
	// Regresando norma espectral
	return maximo;
}



int Es_simetrica (Matriz matriz) {
	int i, j; // Variables de interación
	int simetria = 1; // Suponiendo que es simétrica para después tratar de demostrar lo contrario

	for (i = 0; i < matriz.filas && simetria == 1; i++)
		for (j = 0; j < matriz.columnas && simetria == 1; j++)
			if(matriz.entrada[i * matriz.columnas + j] == matriz.entrada[j * matriz.columnas + i])
				simetria  = 0;
	
	return simetria;
}



void Normalizar (Matriz vector, double norma) {
	int i; // Variable de iteracion

	for (i = 0; i < vector.filas * vector.columnas; i++)
		vector.entrada[i] /= norma;
}

#define EPS 1e-12

// Macro para acceder a elementos
#define MAT(m, i, j) m.entrada[(i) * m.columnas + (j)]

void Invertir_matriz(Matriz matriz) {
    if (matriz.filas != matriz.columnas) {
        printf("Error: la matriz no es cuadrada.\n");
        return;
    }

    int n = matriz.filas;
    int i, j, k;

    // Copia de A (porque la vamos a modificar)
    double *LU = (double *)malloc(sizeof(double) * n * n);
    int *piv = (int *)malloc(sizeof(int) * n);

    if (!LU || !piv) {
        printf("Error de memoria.\n");
        free(LU);
        free(piv);
        return;
    }

    // Inicializar
    for (i = 0; i < n; i++) {
        piv[i] = i;
        for (j = 0; j < n; j++) {
            LU[i*n + j] = MAT(matriz, i, j);
        }
    }

    // Factorización LU con pivoteo parcial
    for (k = 0; k < n; k++) {
        // Buscar pivote
        int max = k;
        double max_val = fabs(LU[k*n + k]);

        for (i = k + 1; i < n; i++) {
            double val = fabs(LU[i*n + k]);
            if (val > max_val) {
                max_val = val;
                max = i;
            }
        }

        if (max_val < EPS) {
            printf("Error: la matriz no es invertible.\n");
            free(LU);
            free(piv);
            return;
        }

        // Intercambio de filas
        if (max != k) {
            for (j = 0; j < n; j++) {
                double tmp = LU[k*n + j];
                LU[k*n + j] = LU[max*n + j];
                LU[max*n + j] = tmp;
            }

            int tmp = piv[k];
            piv[k] = piv[max];
            piv[max] = tmp;
        }

        // Eliminación
        for (i = k + 1; i < n; i++) {
            LU[i*n + k] /= LU[k*n + k];

            for (j = k + 1; j < n; j++) {
                LU[i*n + j] -= LU[i*n + k] * LU[k*n + j];
            }
        }
    }

    // Reservar espacio para la inversa
    double *inv = (double *)malloc(sizeof(double) * n * n);

    if (!inv) {
        printf("Error de memoria.\n");
        free(LU);
        free(piv);
        return;
    }

    // Resolver AX = I columna por columna
    for (j = 0; j < n; j++) {
        double *y = (double *)malloc(sizeof(double) * n);
        double *x = (double *)malloc(sizeof(double) * n);

        // Construir e_j con pivoteo aplicado
        for (i = 0; i < n; i++) {
            y[i] = (piv[i] == j) ? 1.0 : 0.0;
        }

        // Sustitución hacia adelante (Ly = Pb)
        for (i = 0; i < n; i++) {
            for (k = 0; k < i; k++) {
                y[i] -= LU[i*n + k] * y[k];
            }
        }

        // Sustitución hacia atrás (Ux = y)
        for (i = n - 1; i >= 0; i--) {
            for (k = i + 1; k < n; k++) {
                y[i] -= LU[i*n + k] * x[k];
            }
            x[i] = y[i] / LU[i*n + i];
        }

        // Guardar columna j de la inversa
        for (i = 0; i < n; i++) {
            inv[i*n + j] = x[i];
        }

        free(y);
        free(x);
    }

    // Copiar resultado a la matriz original
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            MAT(matriz, i, j) = inv[i*n + j];
        }
    }

    free(LU);
    free(piv);
    free(inv);
}

Matriz Multiplicar_matrices (Matriz matriz_1, Matriz matriz_2) {
	int i, j, k; // Variables para bucles
	Matriz resultado;

	// Inicializando la matriz resultado
	resultado.filas = matriz_1.filas;
	resultado.columnas = matriz_2.columnas;
	resultado.entrada = (double *) malloc(resultado.filas * resultado.columnas * sizeof(double));

	// Multiplicando las matrices
	for (i=0; i < resultado.filas; i++)
		for (j=0; j < resultado.columnas; j++) {
			resultado.entrada[i * resultado.columnas + j] = 0;
			for (k=0; k < matriz_1.columnas; k++)
				resultado.entrada[i * resultado.columnas + j] += matriz_1.entrada[i * matriz_1.columnas + k] * matriz_2.entrada[k * matriz_2.columnas + j];
		}

	return resultado;
}

Matriz SR_matrices (Matriz matriz_1, Matriz matriz_2, int sr) {
	/* Suma o Resta dos matrices */

	int i, j; //Variables para bucles
	Matriz resultado;

	// Inicializando la matriz resultado
	resultado.filas = matriz_1.filas;
	resultado.columnas = matriz_2.columnas;
	resultado.entrada = (double *) malloc(resultado.filas * resultado.columnas * sizeof(double));

	// Sumando las matrices
	for (i=0; i < resultado.filas; i++)
		for (j=0; j < resultado.columnas; j++)
			if (sr == 1)
				resultado.entrada[i * resultado.columnas + j] = matriz_1.entrada[i * resultado.columnas + j] + matriz_2.entrada[i * resultado.columnas + j];
			else
				resultado.entrada[i * resultado.columnas + j] = matriz_1.entrada[i * resultado.columnas + j] - matriz_2.entrada[i * resultado.columnas + j];
	
	return resultado;
}

void Multiplicar_matriz_con_escalar (Matriz matriz, double escalar) {
	int i, j; //Variables para bucles

	for (i=0; i < matriz.filas; i++)
		for (j=0; j < matriz.columnas; j++)
			matriz.entrada[i * matriz.columnas + j] *= escalar;
}

void Transponer_matriz (Matriz *matriz) {
	int i, j; // Variables para bucles
	double temporal; // Variable para intercambiar valores

	for (i=0; i < (*matriz).filas; i++)
		for (j=0; j < (*matriz).columnas; j++) {
			temporal = (*matriz).entrada[i * (*matriz).columnas + j];
			(*matriz).entrada[i * (*matriz).columnas + j] = (*matriz).entrada[j * (*matriz).filas + i];
			(*matriz).entrada[j * (*matriz).filas + i] = temporal;
		}

	i = (*matriz).filas;
	(*matriz).filas = (*matriz).columnas;
	(*matriz).columnas = i;

}



// FUNCIONES PARA FACTORIZACIÓN DE MATRICES --------------------------------------------------------

void Factorizacion_doolittle (Matriz *lower, Matriz *upper, Matriz matriz) {
	int i,j,k; // Variables para iteraciones

	// Inicializando las matrices `lower` y `upper`
	(*lower).filas = matriz.filas;
	(*lower).columnas = matriz.columnas;
	(*lower).entrada = (double *) malloc (sizeof (double) * matriz.filas * matriz.columnas);
	(*upper).filas = matriz.filas;
	(*upper).columnas = matriz.columnas;
	(*upper).entrada = (double *) malloc (sizeof (double) * matriz.filas * matriz.columnas);

	// Comenzando la factorización
	
	// Factorizando la matriz
	for (i = 0; i < (*lower).filas; i++)
		for (j = 0; j < (*lower).columnas; j++) {
			// Asignando valor a la entrada de `upper`
			if (i > j) { // Caso cuando el número de la fila de la entrada es mayor que el de su columna
				(*upper).entrada[i * (*upper).columnas + j] = 0.0;
			}
			else { // Caso cuando el número de la fila de la entrada sea menor o igual al de la columna
				(*upper).entrada[i * (*upper).columnas + j] = matriz.entrada[i * matriz.columnas + j];
				for (k = 0; k < i; k++)
					(*upper).entrada[i * (*upper).columnas + j] -= (*lower).entrada[i * (*lower).columnas + k] * (*upper).entrada[k * (*upper).columnas + j];
			}
			
			// Asignando valor a la entrada de `lower`
			if (i > j) { // Caso cuando el número de la fila de la entrada es mayor que el de su columna
				(*lower).entrada[i * (*lower).columnas + j] = matriz.entrada[i * matriz.columnas + j];
				for (k = 0; k < j; k++)
					(*lower).entrada[i * (*lower).columnas + j] -= (*lower).entrada[i * (*lower).columnas + k] * (*upper).entrada[k * (*upper).columnas + j];
				(*lower).entrada[i * (*lower).columnas + j] /= (*upper).entrada[j * (*upper).columnas + j];
			}
			else if (i == j) { // Caso cuando el número de la fila de la entrada es igual que el de su columna
				(*lower).entrada[i * (*lower).columnas + j] = 1.0;
			}
			else { // en caso contrario
				(*lower).entrada[i * (*lower).columnas + j] = 0.0;
			}
		}
}



Matriz Factorizacion_cholesky (Matriz matriz, int *error) {
	int i, j, m; // Variables para iteraciones
	Matriz lower;

	// Inicializando las dimensiones de `lower`
	lower.filas = matriz.filas;
	lower.columnas = matriz.columnas;
	lower.entrada = (double *) malloc (sizeof(double) * lower.filas * lower.columnas);

	// Obteniendo los valores de las entradas de `lower`
	for (i = 0; i < lower.filas; i++)
		for (j = 0; j < lower.columnas; j++) {
			// Si la fila es menor a la columna
			if (j > i)
				lower.entrada[i * lower.columnas + j] = 0; // Se iguala a ceros
			// En caso contrario
			else {
				// Esto ocurre en caso de que la columna sean igual a la fila o menor que esta

				// Asignando el valo de la entrada con misma fila y columna en la matriz de coeficientes
				lower.entrada[i * lower.columnas + j] = matriz.entrada[i * lower.columnas + j];

				// En caso de ser iguales la columna y la fila
				if (j == i) {
					// Restando cuadrados
					for (m = 0; m < j; m++)
						lower.entrada[i * lower.columnas + j] -= pow (lower.entrada[i * lower.columnas + m], 2.0);

					// Verificando que sea definida positiva
					if (lower.entrada[i * lower.columnas + j] < 0) { // Este caso ocurrirá cuando la matriz no es definida positiva
						*error = 1;
						return matriz;
					} else
						lower.entrada[i * lower.columnas + j] = sqrt (lower.entrada[i * lower.columnas + j]);
				// En caso de que la columna sea menor que la fila
				}
				else {
					for (m = 0; m < j; m++)
						lower.entrada[i * lower.columnas + j] -= lower.entrada[i * lower.columnas + m] * lower.entrada[j * lower.columnas + m];
					lower.entrada[i * lower.columnas + j] /= lower.entrada[j * lower.columnas + j];
				}
			}
		}
	
	// Regresando la matriz factorizada
	return lower;
}



void Factorizacion_crout (Matriz *lower, Matriz *upper, Matriz matriz) {
	int i,j,k; // Variables para iteraciones

	// Inicializando las matrices `lower` y `upper`
	(*lower).filas = matriz.filas;
	(*lower).columnas = matriz.columnas;
	(*lower).entrada = (double *) malloc (sizeof (double) * matriz.filas * matriz.columnas);
	(*upper).filas = matriz.filas;
	(*upper).columnas = matriz.columnas;
	(*upper).entrada = (double *) malloc (sizeof (double) * matriz.filas * matriz.columnas);

	// Factorizando la matriz
	for (i = 0; i < (*lower).filas; i++)
		for (j = 0; j < (*lower).columnas; j++) {
			// Asignando valor a la entrada de `upper`
			if (i > j) { // Caso cuando el número de la fila de la entrada es mayor que el de su columna
				(*upper).entrada[i * (*upper).columnas + j] = 0.0;
			}
			else if (i == j) { // Caso cuando el número de la fila de la entrada es igual que el de su columna
				(*upper).entrada[i * (*upper).columnas + j] = 1.0;
			}
			else { // Caso cuando el número de la fila de la entrada sea menor o igual al de la columna
				(*upper).entrada[i * (*upper).columnas + j] = matriz.entrada[i * matriz.columnas + j];
				for (k = 0; k < i; k++)
					(*upper).entrada[i * (*upper).columnas + j] -= (*lower).entrada[i * (*lower).columnas + k] * (*upper).entrada[k * (*upper).columnas + j];
				(*upper).entrada[i * (*upper).columnas + j] /= (*lower).entrada[i * (*lower).columnas + i];
			}
			
			// Asignando valor a la entrada de `lower`
			if (i >= j) { // Caso cuando el número de la fila de la entrada es mayor o igual que el de su columna
				(*lower).entrada[i * (*lower).columnas + j] = matriz.entrada[i * matriz.columnas + j];
				for (k = 0; k < j; k++)
					(*lower).entrada[i * (*lower).columnas + j] -= (*lower).entrada[i * (*lower).columnas + k] * (*upper).entrada[k * (*upper).columnas + j];
			}
			else { // en caso contrario
				(*lower).entrada[i * (*lower).columnas + j] = 0.0;
			}
		}
}



// OTRAS FUNCIONES ---------------------------------------------------------------------------------

void Imprimir_matriz (Matriz matriz) { // Pensado para imprimir matrices
	int i, j; // Variables para iteraciones

	for (i = 0; i < matriz.filas; i++) {
		printf ("|");
		for (j = 0; j < matriz.columnas; j++)
			printf (" %.10lf |", matriz.entrada[i * matriz.columnas + j]);
		puts ("");
	}
}

void Imprimir_vector (Matriz matriz) { // Pensado para imprimir vectores fila o vectores columna
	int i; // variable para iteraciones

	printf ("( %.10lf ", matriz.entrada[0]);
	for (i = 1; i < matriz.filas * matriz.columnas; i++)
		printf (", %.10lf ", matriz.entrada[i]);
	puts (")");
}



void Copiar_matriz (Matriz original, Matriz *copia) {
	int i; // Variable para iteraciones

	// Copiando dimensiones de la matriz original y liberando memoria de la matriz copia
	(*copia).filas = original.filas;
	(*copia).columnas = original.columnas;
	if ((*copia).entrada != NULL)
		free ((*copia).entrada);
	(*copia).entrada = (double *) malloc (sizeof (double) * (*copia).filas * (*copia).columnas);

	for (i = 0; i < original.filas * original.columnas; i++)
		(*copia).entrada[i] = original.entrada[i];
}
