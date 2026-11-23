#ifndef SEGMENTACION_H
#define SEGMENTACION_H

#include <stdbool.h>

enum {
    KIB = 1024,
    BITS_DESPLAZAMIENTO = 12,
    TAM_VENTANA = 1 << BITS_DESPLAZAMIENTO,
    TAM_VIRTUAL = 16 * KIB,
    TAM_FISICO = 64 * KIB,
    RESERVA_SO = 16 * KIB,
    CANTIDAD_SEGMENTOS = 3
};

typedef struct {
    int id;
    const char *nombre;
    int base;
    int limite;
    bool crece_hacia_abajo;
} Segmento;

typedef enum {
    ACCESO_VALIDO,
    DIRECCION_FUERA_DE_RANGO,
    SEGMENTO_SIN_ASIGNAR,
    FUERA_DEL_LIMITE
} EstadoAcceso;

typedef struct {
    EstadoAcceso estado;
    const Segmento *segmento;
    int id_segmento;
    int desplazamiento;
    int direccion_fisica;
} Traduccion;

extern const Segmento segmentos[CANTIDAD_SEGMENTOS];

const Segmento *buscar_segmento(int id);
Traduccion traducir_direccion(long direccion_virtual);
void mostrar_mapa(void);
void demostrar_compactacion(void);

#endif
