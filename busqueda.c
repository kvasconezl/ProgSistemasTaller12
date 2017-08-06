#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define MAX 100000

pthread_mutex_t mutex;
char **palabras;
int *num_palabras;
char *ruta;
int *numCaracteresLinea;
int bloque;
char **parrafos;
int hilos;

int numero_lineas(char *ruta, int *tam_lineas) {
    if (ruta != NULL) {
        FILE *ar = fopen(ruta, "r");
        int lineas = 0;
        int tam_linea;
        while (!feof(ar)) {
            tam_linea++;
            char c = getc(ar);
            if (c == '\n') {
                if (tam_lineas != NULL) {
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

void *conteo(void *arg) {
    char *str = (char *) arg;
    char *token;
    token = strtok(str, " \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");

    while (token != NULL) {
        if (!strcmp(*(palabras), token)) {
            pthread_mutex_lock(&mutex);
            *(num_palabras) = *(num_palabras) + 1;
            pthread_mutex_unlock(&mutex);
        } else if (!strcmp(*(palabras + 1), token)) {
            pthread_mutex_lock(&mutex);
            *(num_palabras + 1) = *(num_palabras + 1) + 1;
            pthread_mutex_unlock(&mutex);
        } else if (!strcmp(*(palabras + 2), token)) {
            pthread_mutex_lock(&mutex);
            *(num_palabras + 2) = *(num_palabras + 2) + 1;
            pthread_mutex_unlock(&mutex);
        }
        token = strtok(NULL, " \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");
    }

    printf("--*-Palabras encontradas-*-- \nPalabra: %s - > Cantidad: %d\nPalabra: %s - > Cantidad: %d\nPalabra: %s - > Cantidad: %d\n", *(palabras), *(num_palabras), *(palabras + 1), *(num_palabras + 1), *(palabras + 2), *(num_palabras + 2));
    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("USO: ./buscar <RUTA> <HILOS> <PALABRA1> <PALABRA2> <PALABRA3>\n");
        exit(-1);
    }

    //Aqui esta el arreglo de las palabras
    palabras = malloc(3 * sizeof(char *));
    num_palabras = malloc(3 * sizeof(int));
    //Le doy 0 al arreglo de enteros y le doy las palabras al arreglo de palabras
    for (int i = 0; i < 3; i++) {
        *(palabras + i) = argv[3 + i];
        *(num_palabras + i) = 0;
    }
    ruta = argv[1];
    hilos = atoi(argv[2]);
    int numLineas = numero_lineas(ruta, 0);
    numCaracteresLinea = malloc(numLineas * sizeof(int));
    numero_lineas(ruta, numCaracteresLinea);
    int totalCaracteres = 0;
    for (int i = 1; i < numLineas; i++) {
        totalCaracteres += *(numCaracteresLinea + i);
    }

    int bloque = totalCaracteres / hilos;

    int seek = 0;
    parrafos = malloc(hilos * sizeof(char *));

    for (int i = 0; i < hilos; i++) {
        FILE *file = fopen(ruta, "r");
        fseek(file, seek, SEEK_SET);
        *(parrafos + i) = malloc(bloque * sizeof(char));

        for (int j = 0; j < bloque; j++) {
            char c = getc(file);
            *(*(parrafos + i) + j) = c;
        }
        seek = seek + bloque;
    }

    int status;
    pthread_t *threads = malloc(hilos * sizeof(pthread_t));

    for (int i = 0; i < hilos; i++) {
        status = pthread_create(threads + i, NULL, conteo, (void *) *(parrafos + i));
        if (status < 0) {
            fprintf(stderr, "Error al crear el hilo : %d\n", i);
            exit(-1);
        }
    }

    for (int j = 0; j < hilos; j++) {
        int status1 = pthread_join(threads[j], NULL);
        if (status1 < 0) {
            fprintf(stderr, "Error al esperar por el hilo\n");
            exit(-1);
        }
    }

    return (0);
}