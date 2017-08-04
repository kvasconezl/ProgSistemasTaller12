#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 1000000

int numero_lineas(char *ruta, int *tam_lineas) {
	if (ruta != NULL) {
		FILE *ar = fopen(ruta, "r");
		int lineas = 0;
		int tam_linea;
		while (!feof(ar)) {
			tam_linea++;
			char c = getc(ar);
			if (c == '\n') {
				if (tam_lineas[lineas] != NULL) {
					tam_lineas[lineas] = tam_linea;
				}
				lineas++;
				tam_linea = 0;
			}
		}
		fclose(ar);
		return lineas;
	}
	return -1;
}

int main(int argc, char **argv) {
	if (argc != 6) {
		printf("USO: ./buscar <RUTA> <HILOS> <PALABRA1> <PALABRA2> <PALABRA3>\n");
		exit(-1);
	}

	char *palabras = (char *) malloc(3 * 256);
	int num_palabras[3] = {0, 0, 0};



	return 0;
}