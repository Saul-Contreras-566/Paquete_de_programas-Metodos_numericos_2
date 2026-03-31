#include "Funciones_basicas.h"
#include "Opciones.h"

int main () {
	int seleccion;

	// Portada
	puts("PROGRAMA DE MÉTODOS NUMÉRICOS 2\n");
	puts("Integrantes:");
	puts("- Contreras Rojas Emanuel Saúl.");
	puts("- Carmona Maldonado Gibrán Isaí.");
	puts("- Guzmán Ramos Carlos Emilio.");
	ESPERAR_TECLA();

	// Algoritmo
	do {
		LIMPIAR_PANTALLA();
		puts("Seleccione lo que quiere hacer.");
		puts("0. SALIR.");
		puts("1. Resolver un sistemas de ecuaciones no lineales con el método de Broyden.");
		puts("2. Interpolar un punto por medio de diferencias de Newton.");
		puts("Seleccionar:");
		seleccion = Leer_entero_entre(0,2);

		switch (seleccion) {
			case 1: Opcion_1(); break;
			case 2: Opcion_2(); break;
		}
	}
	while (seleccion != 0);

	return 0;
}
