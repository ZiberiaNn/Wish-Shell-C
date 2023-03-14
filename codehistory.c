#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int main() {
    char* line;
    // Leer los comandos del usuario
    while ((line = readline("> ")) != NULL) {
        // Almacenar el comando en el historial
            add_history(line);
        
        printf("Comando ingresado: %s\n", line);
        // Limpiar la línea actual de la consola
    }
    return 0;
}