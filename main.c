#include "segmentacion.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void mostrar_traduccion(long direccion, Traduccion resultado) {
    printf("%5ld | ", direccion);

    switch (resultado.estado) {
    case ACCESO_VALIDO:
        printf("%-6s | %5d | Física: %d\n", resultado.segmento->nombre,
               resultado.desplazamiento, resultado.direccion_fisica);
        break;
    case FUERA_DEL_LIMITE:
        printf("%-6s | %5d | Acceso inválido: fuera del límite\n",
               resultado.segmento->nombre, resultado.desplazamiento);
        break;
    case SEGMENTO_SIN_ASIGNAR:
        printf("Acceso inválido: segmento %d sin asignar\n",
               resultado.id_segmento);
        break;
    case DIRECCION_FUERA_DE_RANGO:
        printf("Acceso inválido: dirección fuera del rango [0, %d)\n",
               TAM_VIRTUAL);
        break;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--mapa") == 0) {
        mostrar_mapa();
        return EXIT_SUCCESS;
    }

    if (argc == 2 && strcmp(argv[1], "--compactar") == 0) {
        demostrar_compactacion();
        return EXIT_SUCCESS;
    }

    if (argc < 2) {
        fprintf(stderr, "Uso: %s DIRECCION [DIRECCION ...]\n", argv[0]);
        fprintf(stderr, "     %s --mapa\n", argv[0]);
        fprintf(stderr, "     %s --compactar\n", argv[0]);
        return EXIT_FAILURE;
    }

    int estado = EXIT_SUCCESS;
    puts("Virt. | Seg.   | Despl | Resultado");

    for (int i = 1; i < argc; i++) {
        char *fin;
        errno = 0;
        long direccion = strtol(argv[i], &fin, 10);

        if (errno == ERANGE || fin == argv[i] || *fin != '\0') {
            fprintf(stderr, "Entrada inválida: %s. Use un entero decimal.\n",
                    argv[i]);
            estado = EXIT_FAILURE;
            continue;
        }
        mostrar_traduccion(direccion, traducir_direccion(direccion));
    }
    return estado;
}
