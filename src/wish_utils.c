#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "wish_utils.h"

void execute_exit(int value){
	exit(0);
}

void execute_cd(char *newpath){
	char *path = strtok_r(newpath, " ", &newpath);
	printf("path: %s", path);
	//Intenta ir a la dirección, si la dirección no es valida, imprime un error.
	if(chdir(path) != 0){
		write(STDERR_FILENO, error_message, strlen(error_message));
		return;
	}
	chdir(path);
}

void execute_path(){
	printf("path executed\n");
}
