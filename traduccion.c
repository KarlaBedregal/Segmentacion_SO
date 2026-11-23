#include "segmentacion.h"

#include <stddef.h>

Traduccion traducir_direccion(long direccion_virtual) {
    Traduccion resultado = {.estado = DIRECCION_FUERA_DE_RANGO};

    if (direccion_virtual < 0 || direccion_virtual >= TAM_VIRTUAL) {
        return resultado;
    }

    // Los 2 bits altos identifican el segmento y los 12 bajos dan el
    // desplazamiento.
    resultado.id_segmento = (int)(direccion_virtual >> BITS_DESPLAZAMIENTO);
    resultado.desplazamiento = (int)(direccion_virtual & (TAM_VENTANA - 1));
    resultado.segmento = buscar_segmento(resultado.id_segmento);

    if (resultado.segmento == NULL) {
        resultado.estado = SEGMENTO_SIN_ASIGNAR;
        return resultado;
    }

    const Segmento *segmento = resultado.segmento;
    int minimo = 0;
    int maximo = segmento->limite;

    if (segmento->crece_hacia_abajo) {
        // La base del stack esta justo despues del ultimo byte valido.
        resultado.desplazamiento -= TAM_VENTANA;
        minimo = -segmento->limite;
        maximo = 0;
    }

    int desplazamiento = resultado.desplazamiento;
    if (desplazamiento < minimo || desplazamiento >= maximo) {
        resultado.estado = FUERA_DEL_LIMITE;
        return resultado;
    }

    resultado.direccion_fisica = segmento->base + desplazamiento;
    resultado.estado = ACCESO_VALIDO;
    return resultado;
}
