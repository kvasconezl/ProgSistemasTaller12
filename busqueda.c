#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define MAX 100000

extern char *strtok_r(char *str, const char *delim, char **saveptr);

pthread_mutex_t mutex;
char **palabras;
int num_palabras[3] = {0, 0, 0};
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

void *mostrar_palabras(void *arg) {
    while (1) {
        sleep(1);
        printf("--*-Palabras encontradas-*-- \nPalabra: %s - > Cantidad: %d\nPalabra: %s - > Cantidad: %d\nPalabra: %s - > Cantidad: %d\n",
               *(palabras), num_palabras[0], *(palabras + 1), num_palabras[1], *(palabras + 2), num_palabras[2]);
    }
    return (void *) 0;
}

void *conteo(void *arg) {
    char *str = (char *) arg;
    char *token;
    token = strtok_r(str, " \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;", &str);

    while (token != NULL) {
        sleep(1);   //Comentar para ver resultados finales del conteo
        if (!strcmp(*(palabras), token)) {
            pthread_mutex_lock(&mutex);
            num_palabras[0]++;
            pthread_mutex_unlock(&mutex);
        } else if (!strcmp(*(palabras + 1), token)) {
            pthread_mutex_lock(&mutex);
            num_palabras[1]++;
            pthread_mutex_unlock(&mutex);
        } else if (!strcmp(*(palabras + 2), token)) {
            pthread_mutex_lock(&mutex);
            num_palabras[2]++;
            pthread_mutex_unlock(&mutex);
        }
        token = strtok_r(NULL, " \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;", &str);
    }

    return (void *) 0;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("USO: ./buscar <RUTA> <HILOS> <PALABRA1> <PALABRA2> <PALABRA3>\n");
        exit(-1);
    }

    palabras = malloc(3 * sizeof(char *));

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

    pthread_t *t_mostrar = malloc(sizeof(pthread_t));
    int stat_mostrar1, stat_mostrar2;
    int status;
    pthread_t *threads = malloc(hilos * sizeof(pthread_t));

    stat_mostrar1 = pthread_create(t_mostrar, NULL, mostrar_palabras, NULL);

    if (stat_mostrar1 != 0) {
        fprintf(stderr, "Error al crear el hilo para mostrar palabras.\n");
    }

    for (int i = 0; i < hilos; i++) {
        status = pthread_create(threads + i, NULL, conteo, (void *) *(parrafos + i));
        if (status != 0) {
            fprintf(stderr, "Error al crear el hilo : %d\n", i);
            exit(-1);
        }
    }

    for (int j = 0; j < hilos; j++) {
        int status1 = pthread_join(threads[j], NULL);
        if (status1 != 0) {
            fprintf(stderr, "Error al esperar por el hilo\n");
            exit(-1);
        }
    }

    stat_mostrar2 = pthread_join(*t_mostrar, NULL);
    if (stat_mostrar2 != 0) {
        fprintf(stderr, "Error al esperar por el hilo para mostrar palabras.\n");
    }

    return (0);
}