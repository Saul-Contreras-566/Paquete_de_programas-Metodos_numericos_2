/* OPCION 1: Solucionador de sistemas de ecuaciones por el método de Broyden. */

// FUNCIONES PARA EVALUAR UN VECTOR EN UN SISTEMA

Matriz F_1 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 1;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = pow(vector.entrada[0], 2.0) + vector.entrada[0] * vector.entrada[1] + 2.0 * pow(vector.entrada[1], 2.0) - 5.0;
	resultado.entrada[1] = 5.0 * vector.entrada[1] - 2.0 * vector.entrada[0] * pow(vector.entrada[1], 2.0) + 3.0;

	return resultado;
}

Matriz F_2 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 1;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = pow(vector.entrada[0], 2.0) - 3.0 * pow(vector.entrada[1], 2.0) - 10;
	resultado.entrada[1] = 2 * pow(vector.entrada[1], 2.0) - 3.0 * vector.entrada[0] * vector.entrada[1] + 1.0;

	return resultado;
}

Matriz F_3 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 1;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = 3.0 * pow(vector.entrada[0], 2.0) + pow(vector.entrada[1], 2.0) - 8 * vector.entrada[1] + 2 * pow(vector.entrada[2], 2.0) - 5.0;
	resultado.entrada[1] = -2.0 * pow(vector.entrada[0], 2.0) - 12 * vector.entrada[0] + pow(vector.entrada[1], 2.0) - 3 * pow(vector.entrada[2], 2.0) + 10.0;
	resultado.entrada[2] = - vector.entrada[0] + 2 * vector.entrada[1] + 5 * vector.entrada[2];

	return resultado;
}

Matriz F_4 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 1;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = pow(vector.entrada[0], 2.0) + pow(vector.entrada[1], 2.0) - 2 * vector.entrada[2] + 3.0;
	resultado.entrada[1] = vector.entrada[0] + vector.entrada[1] + vector.entrada[2] - 5.0;
	resultado.entrada[2] = pow(vector.entrada[0], 2.0) - pow(vector.entrada[1], 2.0) + pow(vector.entrada[2], 2.0) - 9.0;

	return resultado;
}

// JACOBIANAS DE LOS SISTEMAS

Matriz J_1 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 2;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = 2.0 * vector.entrada[0] + vector.entrada[1]; 
	resultado.entrada[1] = vector.entrada[0] + 4.0 * vector.entrada[1];
	resultado.entrada[2] = - 2.0 * pow(vector.entrada[1], 2.0);
	resultado.entrada[3] = 5.0 - 4.0 * vector.entrada[0] * vector.entrada[1];

	return resultado;
}

Matriz J_2 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 2;
	resultado.columnas = 2;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = 2.0 * vector.entrada[0];
	resultado.entrada[1] = -6.0 * vector.entrada[1];
	resultado.entrada[2] = -3.0 * vector.entrada[1];
	resultado.entrada[3] = 4.0 * vector.entrada[1] - 3.0 * vector.entrada[0];

	return resultado;
}

Matriz J_3 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 3;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = 6.0 * vector.entrada[0];
	resultado.entrada[1] = 2.0 * vector.entrada[1] - 8.0;
	resultado.entrada[2] = 4.0 * vector.entrada[2];
	resultado.entrada[3] = -4.0 * vector.entrada[0] - 12.0;
	resultado.entrada[4] = 2.0 * vector.entrada[1];
	resultado.entrada[5] = -6.0 * vector.entrada[2];
	resultado.entrada[6] = -1.0;
	resultado.entrada[7] = 2.0;
	resultado.entrada[8] = 5.0;

	return resultado;
}

Matriz J_4 (Matriz vector) {
	Matriz resultado;
	resultado.filas = 3;
	resultado.columnas = 3;
	resultado.entrada = (double *) malloc( resultado.filas * resultado.columnas * sizeof(double));

	resultado.entrada[0] = 2.0 * vector.entrada[0];
	resultado.entrada[1] = 2.0 * vector.entrada[1];
	resultado.entrada[2] = -2.0;
	resultado.entrada[3] = 1.0;
	resultado.entrada[4] = 1.0;
	resultado.entrada[5] = 1.0;
	resultado.entrada[6] = 2.0 * vector.entrada[0];
	resultado.entrada[7] = -2.0 * vector.entrada[1];
	resultado.entrada[8] = 2.0 * vector.entrada[2];

	return resultado;
}

// SELECTORES DE OPCIONES

Matriz Sistema (Matriz vector, int enesimo) {
	switch (enesimo) {
		case 1: return F_1(vector);
		case 2: return F_2(vector);
		case 3: return F_3(vector);
	}
	return F_4(vector);
}

Matriz Jacobiana (Matriz vector, int enesimo) {
	switch (enesimo) {
		case 1: return J_1(vector);
		case 2: return J_2(vector);
		case 3: return J_3(vector);
	}
	return J_4(vector);
}

// ITERADOR Y MENÚ DE SELECCIÓN

void Imprimir_iteracion(int iteracion, Matriz vector, double norma, double tolerancia) {
	printf("-- Iteración %d --\n", iteracion);
	printf("Vector: ");
	Imprimir_vector(vector);
	printf("Norma espectral: %lf\n", norma);
	printf("¿Seguir?: %s\n\n", (norma < tolerancia ? "Parar" : "Seguir"));
}

void Iterador(int seleccion, int max_iteraciones, double tolerancia, Matriz vector) {
	int i; // Variable para bucle
	double norma;
	Matriz x_0, x_1, inc_x; // Aproximaciones anterior, actual e incremento respectivamente del vector solución
	Matriz f_0, f_1, inc_f; // Aproximaciones anterior, actual e incremento respectivamente del vector solución evaluado en el sistema de ecuaciones
	Matriz j; // Jacobiana del sistema
	Matriz a_0, a_1; // Aproximaciones anterior y actual respectivamente de la inversa de la jacobiana
	Matriz temporal_1, temporal_2, temporal_3, temporal_4, temporal_5; // Para calculos temporales

	// Inicializnado las dimensiones de algunas matrices
	x_0.filas = vector.filas;
	x_0.columnas = vector.columnas;
	x_1.filas = vector.filas;
	x_1.columnas = vector.columnas;
	f_0.filas = vector.filas;
	f_0.columnas = vector.columnas;
	f_1.filas = vector.filas;
	f_1.columnas = vector.columnas;
	a_0.filas = vector.filas;
	a_0.columnas = vector.filas;
	a_1.filas = vector.filas;
	a_1.columnas = vector.filas;

	// Calculando la iteración 0
	f_1 = Sistema(vector, seleccion);
	norma = Norma_espectral(f_1.filas * f_1.columnas, f_1.entrada);
	Imprimir_iteracion(0, vector, norma, tolerancia);

	// Reajustando x y f para la siguiente iteración
	x_0.entrada = vector.entrada;
	f_0.entrada = f_1.entrada;

	// Calculando la iteración 1 con la jacobiana
	j = Jacobiana(vector, seleccion);
	Invertir_matriz(j);
	temporal_1 = Multiplicar_matrices(j, f_1);
	x_1 = SR_matrices(x_0, temporal_1, -1); // Restando vector inicial y temporal
	f_1 = Sistema(x_1, seleccion);
	norma = Norma_espectral(f_1.filas * f_1.columnas, f_1.entrada);
	a_0.entrada = j.entrada;
	Imprimir_iteracion(1, x_1, norma, tolerancia);

	// Calculando las demás iteraciones
	for (i = 2; i <= max_iteraciones && norma > tolerancia; i++) {

		// Calculando incrementos
		inc_x = SR_matrices(x_1, x_0, -1); // Restando vectores
		inc_f = SR_matrices(f_1, f_0, -1); // Restando evaluaciones de vectores en el sistema
		// Calculando la aproximación de la inversa de la jacobiana
		temporal_1 = Multiplicar_matrices(a_0, inc_f);
			// Liberando memoria
			free(inc_f.entrada);
		Transponer_matriz(&inc_x);
		temporal_2 = Multiplicar_matrices(inc_x, a_0);
		temporal_3 = Multiplicar_matrices(inc_x, temporal_1);
		Transponer_matriz(&inc_x);
		temporal_4 = SR_matrices(inc_x, temporal_1, -1);
			// Libreando memoria
			free(inc_x.entrada);
			free(temporal_1.entrada);
		temporal_5 = Multiplicar_matrices(temporal_4, temporal_2);
			// Liberando memoria
			free(temporal_2.entrada);
			free(temporal_4.entrada);
		Multiplicar_matriz_con_escalar(temporal_5, 1.0 / temporal_3.entrada[0]);
			// Liberando memoria
			free(temporal_3.entrada);
		a_1 = SR_matrices(a_0, temporal_5, 1);
			// Liberando memoria
			free(temporal_5.entrada);

		// Reajustando x y f para la siguiente iteración
		free(x_0.entrada); x_0.entrada = x_1.entrada;
		free(f_0.entrada); f_0.entrada = f_1.entrada;
		free(a_0.entrada); a_0.entrada = a_1.entrada;

		// Siguiendo esquema iterativo
		temporal_1 = Multiplicar_matrices(a_1, f_1);
		x_1 = SR_matrices(x_0, temporal_1, -1); // Restando x_0 - temporal_1
			// Liberando memoria
			free(temporal_1.entrada);

		// Imprimiendo iteración
		f_1 = Sistema(x_1, seleccion);
		norma = Norma_espectral(f_1.filas * f_1.columnas, f_1.entrada);
		Imprimir_iteracion(i, x_1, norma, tolerancia);

	}

	// Imprimiendo resultados
	puts("-- PARANDO --");
	if (i == max_iteraciones && norma > tolerancia) puts("- Se alcanzó el máximo de iteraciones. Puede que no converja.");
	else puts("- Se alcanzó la tolerancia.");
	puts("Resultados de las iteraciones:");
	printf("Mejor aproximación: "); Imprimir_vector(x_1);
	printf("Norma de F: %lf\n", norma);
}

void Opcion_1() {
	int seleccion, respuesta;
	int max_iteraciones; // Para guardar el máximo de iteraciones
	double tolerancia;
	Matriz vector;
   
	do {
		// Impresión del menú de selección
		LIMPIAR_PANTALLA();
		puts("¿Cuál es la ecuación que quiere resolver?");
		puts("0. REGRESAR AL MENÚ PRINCIPAL.");
		puts("");
		puts("1.");
		puts("x^2 + xy + 2y^2 - 5 = 0");
		puts("	 5y - 2xy^2 + 3 = 0");
		puts("");
		puts("2.");
		puts("x^2 - 3y^2 - 10 = 0");
		puts(" 2y^2 - 3xy + 1 = 0");
		puts("");
		puts("3.");
		puts("   3x^2 + y^2 - 8y + 2z^2 - 5 = 0");
		puts("-2x^2 - 12x + y^2 - 3z^2 + 10 = 0");
		puts("				 -x + 2y + 5z = 0");
		puts("");
		puts("4.");
		puts(" x^2 + y^2 - 2z + 3 = 0");
		puts("	  x + y + z - 5 = 0");
		puts("x^2 - y^2 + z^2 - 9 = 0");
		puts("");
		puts("Seleccionar:");
		seleccion = Leer_entero_entre(0,4);

		if(seleccion == 0) return;

		do {
			// Pidiendo el máximo de iteraciones y la tolerancia
			puts("Inserte el máximo número de iteraciones:");
			max_iteraciones = Leer_entero_que_sea(">", 0);
			puts("Inserte la tolerancia a usar sobre la norma espectral de F(X^{(k)})");
			tolerancia = Leer_real_que_sea(">", 0);

			// Leyendo el vector inicial
			// Estableciendo dimensiones del vector incial
			vector.columnas = 1;
			if(seleccion == 1 || seleccion == 2) vector.filas = 2;
			else if(seleccion == 3 || seleccion == 4) vector.filas = 3;
			// Leyendo las entradas
			puts("Inserte las entradas del vector inicial:");
			vector.entrada = Leer_entradas_de_matriz(vector.filas, vector.columnas);
			
			// Ejecutando el iterador
			Iterador(seleccion, max_iteraciones, tolerancia, vector);

			// Preguntando por volver a intentar con otro punto inicial
			respuesta = Pregunta_cerrada("¿Quiere probar con otro punto inicial?");

			LIMPIAR_PANTALLA();
		}
		while (respuesta == 1);

		// Preguntando por probar con otro sistema
		respuesta = Pregunta_cerrada("¿Quiere solucionar otro sistema de ecuaciones?");
	}
	while (respuesta == 1);
}
