#include "segmentacion.h"

#include <stddef.h>

// Orden por direccion fisica para recorrer el mapa de menor a mayor.
const Segmento segmentos[CANTIDAD_SEGMENTOS] = {
    {.id = 3,
     .nombre = "Stack",
     .base = 28 * KIB,
     .limite = 2 * KIB,
     .crece_hacia_abajo = true},
    {.id = 0,
     .nombre = "Código",
     .base = 32 * KIB,
     .limite = 2 * KIB,
     .crece_hacia_abajo = false},
    {.id = 1,
     .nombre = "Heap",
     .base = 34 * KIB,
     .limite = 3 * KIB,
     .crece_hacia_abajo = false},
};

const Segmento *buscar_segmento(int id) {
    for (int i = 0; i < CANTIDAD_SEGMENTOS; i++) {
        if (segmentos[i].id == id) {
            return &segmentos[i];
        }
    }
    return NULL;
}
