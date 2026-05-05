# Definiciones básicas
program_name = Paquete_de_programas-Métodos_numéricos-v2026_05_04
compiler_flags = -Wall -Werror -Wextra
linkers = -lSDL2main -lSDL2_gfx -lSDL2 -lm

# Definiendo directorios
source_path = source
headers_path = headers
outputs_path = outputs

# Definiendo archivos de código fuente
sources = $(source_path)/Funciones_basicas.c \
          $(source_path)/Matrices.c \
          $(source_path)/Opcion_1.c \
          $(source_path)/Opcion_2.c \
          $(source_path)/Opcion_3.c \
          $(source_path)/main.c





all:
	gcc $(compiler_flags) -I$(headers_path) $(sources) -o $(program_name)-Linux $(linkers)

windows:
	x86_64-w64-mingw32-gcc $(compiler_flags) -I$(headers_path) $(sources) -o $(program_name)-Windows.exe -lmingw32 $(linkers)
