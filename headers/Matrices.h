/*
Funciones con matrices.
*/

#ifndef MATRICES
#define MATRICES

typedef struct {
	int filas;
	int columnas;
	double *entrada;
} Matriz;

// Funciones básicas con matrices
double *Leer_entradas_de_matriz (int filas, int columnas);
Matriz Leer_matriz ();
double Determinante (Matriz matriz);
double Norma_espectral (int numero_entradas, double *entrada);
void Invertir_matriz (Matriz matriz);
Matriz Multiplicar_matrices (Matriz matriz_1, Matriz matriz_2);
Matriz SR_matrices (Matriz matriz_1, Matriz matriz_2, int sr);
void Multiplicar_matriz_con_escalar (Matriz matriz, double escalar);
void Transponer_matriz (Matriz *matriz);

// Funciones para factorización de matrices
void Factorizacion_doolittle (Matriz *lower, Matriz *upper, Matriz matriz);
Matriz Factorizacion_cholesky (Matriz matriz, int *error);
void Factorizacion_crout (Matriz *lower, Matriz *upper, Matriz matriz);

// Otras funciones
void Imprimir_matriz (Matriz matriz); // Imprime en forma de matriz
void Imprimir_vector (Matriz matriz); // Pensado para imprimir vectores fila o vectores columna
void Copiar_matriz (Matriz original, Matriz *copia);

#endif
