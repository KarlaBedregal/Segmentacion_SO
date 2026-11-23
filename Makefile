CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -Werror -O2
FUENTES = main.c segmentos.c traduccion.c mapa.c

.PHONY: all clean
all: segmentacion

segmentacion: $(FUENTES) segmentacion.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(FUENTES) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) segmentacion
