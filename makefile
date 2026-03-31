# Definiciones básicas
program_name = Paquete_de_programas-Métodos_numéricos
compiler_flags = -Wall -Werror -Wextra

# Definiendo directorios
source_path = source
headers_path = headers
outputs_path = outputs

# Definiendo archivos de código fuente
sources = $(source_path)/Funciones_basicas.c \
          $(source_path)/Matrices.c \
          $(source_path)/Opcion_1.c \
          $(source_path)/Opcion_2.c \
          $(source_path)/main.c





all:
	gcc $(compiler_flags) -I$(headers_path) $(sources) -o $(program_name)-Linux -lm

windows:
	# Para compilar para Windows desde un sistema operativo tipo UNIX.
	x86_64-w64-mingw32-gcc $(compiler_flags) -I$(headers_path) $(sources) -o $(program_name)-Windows.exe -lm
