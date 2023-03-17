#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Debe ingresar el nombre del archivo como argumento.\n");
        return 1;
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    while (1) {
        sleep(1);
        read = getline(&line, &len, fp);
        printf("%s", line);
    }

    free(line);
    fclose(fp);

    return 0;
}