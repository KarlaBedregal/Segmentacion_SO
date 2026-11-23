#include "segmentacion.h"

#include <stdio.h>

static void mostrar_intervalo(int inicio, int fin, const char *nombre) {
    printf("  [%5d, %5d)  %s\n", inicio, fin, nombre);
}

void mostrar_mapa(void) {
    puts("Memoria física OSTEP (en bytes)");
    mostrar_intervalo(0, RESERVA_SO, "Sistema operativo");

    int cursor = RESERVA_SO;
    for (int i = 0; i < CANTIDAD_SEGMENTOS; i++) {
        const Segmento *segmento = &segmentos[i];
        int inicio = segmento->base;
        int fin = inicio + segmento->limite;

        if (segmento->crece_hacia_abajo) {
            fin = segmento->base;
            inicio = fin - segmento->limite;
        }

        if (cursor < inicio) {
            mostrar_intervalo(cursor, inicio, "Libre");
        }
        mostrar_intervalo(inicio, fin, segmento->nombre);
        cursor = fin;
    }

    if (cursor < TAM_FISICO) {
        mostrar_intervalo(cursor, TAM_FISICO, "Libre");
    }
}

typedef struct {
    int base;
    int tamano;
} Bloque;

typedef struct {
    int total;
    int mayor_hueco;
    int primera_base_disponible;
} EspacioLibre;

static EspacioLibre medir_espacio(const Bloque bloques[], int cantidad,
                                  int solicitud) {
    EspacioLibre espacio = {.primera_base_disponible = -1};
    int cursor = RESERVA_SO;

    for (int i = 0; i <= cantidad; i++) {
        // El ultimo hueco termina al final de la memoria fisica.
        int fin_hueco = i < cantidad ? bloques[i].base : TAM_FISICO;
        int tamano = fin_hueco - cursor;
        espacio.total += tamano;
        if (tamano > espacio.mayor_hueco) {
            espacio.mayor_hueco = tamano;
        }
        if (espacio.primera_base_disponible < 0 && tamano >= solicitud) {
            espacio.primera_base_disponible = cursor;
        }
        if (i < cantidad) {
            cursor = bloques[i].base + bloques[i].tamano;
        }
    }
    return espacio;
}

static void probar_solicitud(const char *etapa, const Bloque bloques[],
                             int cantidad, int solicitud) {
    EspacioLibre espacio = medir_espacio(bloques, cantidad, solicitud);
    printf("%s: %d KiB libres | mayor hueco: %d KiB\n", etapa,
           espacio.total / KIB, espacio.mayor_hueco / KIB);

    int base = espacio.primera_base_disponible;
    if (base < 0) {
        puts("  Solicitud rechazada");
    } else {
        printf("  La solicitud cabe en [%d, %d) KiB\n", base / KIB,
               (base + solicitud) / KIB);
    }
}

void demostrar_compactacion(void) {
    // Caso de compactacion de OSTEP, figura 16.6.
    Bloque bloques[] = {
        {24 * KIB, 8 * KIB}, {36 * KIB, 8 * KIB}, {56 * KIB, 8 * KIB}};
    int cantidad = (int)(sizeof bloques / sizeof bloques[0]);
    int solicitud = 20 * KIB;

    printf("Solicitud: segmento de %d KiB\n", solicitud / KIB);
    probar_solicitud("Antes", bloques, cantidad, solicitud);

    int cursor = RESERVA_SO;
    printf("\nCompactación (bases en KiB):");
    for (int i = 0; i < cantidad; i++) {
        printf("  %d -> %d", bloques[i].base / KIB, cursor / KIB);
        bloques[i].base = cursor;
        cursor += bloques[i].tamano;
    }
    putchar('\n');

    probar_solicitud("Después", bloques, cantidad, solicitud);
}
