#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

extern char *mypath[];
extern char error_message[];
#define MAX_FILENAME 1024
int num_paths = 2;

void execute_exit(int value){
	exit(0);
}

void execute_cd(char *newpath){
	char *path = strtok_r(newpath, " ", &newpath);;
	//Intenta ir a la dirección, si la dirección no es valida, imprime un error.
	if(chdir(path) != 0){
		write(STDERR_FILENO, error_message, strlen(error_message));
		return;
	}
	chdir(path);
}

void execute_path( char *newpath){
char *full_path = malloc(MAX_FILENAME);
    if (full_path == NULL) {
        write(STDERR_FILENO, error_message, strlen(error_message));
		return;
    }

    for (int i = 0; i < num_paths; i++) {
        snprintf(full_path, MAX_FILENAME, "%s/%s", mypath[i], newpath);
        if (access(full_path, X_OK) == 0) {
			fprintf(stderr, "El archivo %s se encuentra en la ruta de búsqueda\n", newpath);
			free(full_path);
            return;
        }
    }

    write(STDERR_FILENO, error_message, strlen(error_message));
    free(full_path);
}
