# Segmentación de memoria

Trabajo del curso de **Sistemas Operativos** de la Escuela Profesional de Ingeniería de Sistemas, Universidad Nacional de San Agustín de Arequipa (UNSA).

**Docente** · Norman Patrick Harvey Arce

**Integrantes**

- Aliaga Chaiña Sandra Gabriela
- Bedregal Coaguila Karla Miluska
- Llaique Chullunquia Jack Franco
- Mamani Cespedes Jhonatan Benjamin

## El simulador

El programa está escrito en C y reproduce ejemplos del capítulo 16 de *Operating Systems: Three Easy Pieces* (OSTEP). Representa los segmentos de código, *heap* y *stack*. Para cada dirección virtual, identifica el segmento, calcula el desplazamiento y comprueba si está dentro del límite. Si el acceso es válido, muestra la dirección física en bytes.

El programa comprueba rangos de direcciones. Los permisos de lectura, escritura y ejecución forman parte de la explicación teórica.

## Compilación y uso

Se necesita `make` y un compilador compatible con C17. Desde la raíz del repositorio, se compila y se prueban tres direcciones virtuales en decimal. El programa acepta una o varias direcciones separadas por espacios. Se pueden cambiar en cada ejecución.

```sh
make
./segmentacion 4200 7168 15360
```

| Dirección virtual | Resultado |
| ---: | --- |
| 4200 | Se traduce a la dirección física 34920. |
| 7168 | Se rechaza porque el desplazamiento alcanza el límite del *heap*. |
| 15360 | Se traduce a la dirección física 27648 con un desplazamiento de −1024 bytes en el *stack*. |

Por ejemplo, se pueden probar una dirección de código, el último byte válido del *heap*, el primer byte fuera de su límite y una dirección de un segmento sin asignar, en ese orden.

```sh
./segmentacion 100 7167 7168 8192
```

```sh
./segmentacion --mapa
./segmentacion --compactar
```

`--mapa` muestra los intervalos físicos ocupados y libres del ejemplo de traducción. `--compactar` usa un segundo mapa. En ese caso, una solicitud de 20 KiB se rechaza porque el mayor hueco mide 12 KiB. Al compactar el mapa, los 24 KiB libres quedan reunidos y la solicitud cabe.

Las bases y los tamaños de los segmentos se definen en [`segmentos.c`](segmentos.c). El mapa y la solicitud de 20 KiB del caso de compactación se definen en [`mapa.c`](mapa.c). Para cambiar esos valores, se editan los archivos y se vuelve a ejecutar `make`.

## Archivos principales

| Archivo | Función |
| --- | --- |
| [`main.c`](main.c) | Lee las opciones y las direcciones, y muestra los resultados. |
| [`segmentacion.h`](segmentacion.h) | Declara los tipos, las constantes y las funciones compartidas. |
| [`segmentos.c`](segmentos.c) | Define los segmentos del ejemplo. |
| [`traduccion.c`](traduccion.c) | Valida las direcciones y calcula su ubicación física. |
| [`mapa.c`](mapa.c) | Muestra el mapa físico y el ejemplo de compactación. |

## Referencia

Remzi H. Arpaci-Dusseau y Andrea C. Arpaci-Dusseau. *Operating Systems: Three Easy Pieces*, capítulo 16. [Segmentation](https://pages.cs.wisc.edu/~remzi/OSTEP/vm-segmentation.pdf).
