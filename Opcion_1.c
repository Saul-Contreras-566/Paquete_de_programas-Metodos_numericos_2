/* OPCION 1: Solucionador de sistemas de ecuaciones por el método de Broyden. */

// FUNCIONES PARA EVALUAR UN VECTOR EN UN SISTEMA

Matriz F_1 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 1;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = pow(vector.entradas[0], 2.0) + vector.entradas[0] * vector.entradas[1] + 2.0 * pow(vector.entradas[1], 2.0) - 5.0;
	resultado.entradas[1] = 5.0 * vector.entradas[1] - 2.0 * vector.entradas[0] * pow(vector.entradas[1], 2.0) + 3.0;

	return resultado;
}

Matriz F_2 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 1;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = pow(vector.entradas[0], 2.0) - 3.0 * pow(vector.entradas[1], 2.0) - 10;
	resultado.entradas[1] = 2 * pow(vector.entradas[1], 2.0) - 3.0 * vector.entradas[0] * vector.entradas[1] + 1.0;

	return resultado;
}

Matriz F_3 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 1;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = 3.0 * pow(vector.entradas[0], 2.0) + pow(vector.entradas[1], 2.0) - 8 * vector.entradas[1] + 2 * pow(vector.entradas[2], 2.0) - 5.0;
	resultado.entradas[1] = -2.0 * pow(vector.entradas[0], 2.0) - 12 * vector.entradas[0] + pow(vector.entradas[1], 2.0) - 3 * pow(vector.entradas[2], 2.0) + 10.0;
	resultado.entradas[2] = - vector.entradas[0] + 2 * vector.entradas[1] + 5 * vector.entradas[2];

	return resultado;
}

Matriz F_4 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 1;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = pow(vector.entradas[0], 2.0) + pow(vector.entradas[1], 2.0) - 2 * vector.entradas[2] + 3.0;
	resultado.entradas[1] = vector.entradas[0] + vector.entradas[1] + vector.entradas[2] - 5.0;
	resultado.entradas[2] = pow(vector.entradas[0], 2.0) - pow(vector.entradas[1], 2.0) + pow(vector.entradas[2], 2.0) - 9.0;

	return resultado;
}

// JACOBIANAS DE LOS SISTEMAS

Matriz J_1 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 2;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = 2.0 * vector.entradas[0] + vector.entradas[1]; 
	resultado.entradas[1] = vector.entradas[0] + 4.0 * vector.entradas[1];
	resultado.entradas[2] = - 2.0 * pow(vector.entradas[1], 2.0);
	resultado.entradas[3] = 5.0 - 4.0 * vector.entradas[0] * vector.entradas[1];

	return resultado;
}

Matriz J_2 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 2;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = 2.0 * vector.entradas[0];
	resultado.entradas[1] = -6.0 * vector.entradas[1];
	resultado.entradas[2] = -3.0 * vector.entradas[1];
	resultado.entradas[3] = 4.0 * vector.entradas[1] - 3.0 * vector.entradas[0];

	return resultado;
}

Matriz J_3 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 3;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = 6.0 * vector.entradas[0];
	resultado.entradas[1] = 2.0 * vector.entradas[1] - 8.0;
	resultado.entradas[2] = 4.0 * vector.entradas[2];
	resultado.entradas[3] = -4.0 * vector.entradas[0] - 12.0;
	resultado.entradas[4] = 2.0 * vector.entradas[1];
	resultado.entradas[5] = -6.0 * vector.entradas[2];
	resultado.entradas[6] = -1.0;
	resultado.entradas[7] = 2.0;
	resultado.entradas[8] = 5.0;

	return resultado;
}

Matriz J_4 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 3;
	resultado.entradas = (double) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entradas[0] = 2.0 * vector.entradas[0];
	resultado.entradas[1] = 2.0 * vector.entradas[1];
	resultado.entradas[2] = -2.0;
	resultado.entradas[3] = 1.0
	resultado.entradas[4] = 1.0
	resultado.entradas[5] = 1.0
	resultado.entradas[6] = 2.0 * vector.entradas[0];
	resultado.entradas[7] = -2.0 * vector.entradas[1];
	resultado.entradas[8] = 2.0 * vector.entradas[2];

	return resultado;
}

// SELECTORES DE OPCIONES

Matriz Sistema (Matriz vector, int enesimo) {
	switch (enesimo) {
		case 1: return F_1(vector);
		case 2: return F_2(vector);
		case 3: return F_3(vector);
		case 4: return F_4(vector);
	}
}

Matriz Jacobiana (Matriz vector, int enesimo) {
	switch (enesimo) {
		case 1: return J_1(vector);
		case 2: return J_2(vector);
		case 3: return J_3(vector);
		case 4: return J_4(vector);
	}
}

// ITERADOR

