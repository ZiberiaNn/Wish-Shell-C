#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_HISTORY 20 // Máximo número de comandos en el historial

int main() {
    char* line;
    char* history[MAX_HISTORY];
    int i, history_index = 0;

    // Configurar la biblioteca readline
    rl_bind_key('\t', rl_complete);
    using_history();

    // Leer los comandos del usuario
    while ((line = readline("> ")) != NULL) {
        // Almacenar el comando en el historial
        if (*line) {
            add_history(line);
            if (history_index < MAX_HISTORY) {
                history[history_index++] = line;
            } else {
                free(history[0]);
                for (i = 0; i < history_index - 1; i++) {
                    history[i] = history[i + 1];
                }
                
                history[history_index - 1] = line;
            }
        } else {
            free(line);
        }

        // Ejecutar el comando
        printf("Comando ingresado: %s\n", line);

        // Limpiar la línea actual de la consola
        rl_clear_message();

        // Actualizar el índice del historial
        if (history_index >= MAX_HISTORY) {
            history_index = MAX_HISTORY - 1;
        }
    }

    // Liberar la memoria utilizada por los comandos en el historial
    for (i = 0; i < history_index; i++) {
        free(history[i]);
    }

    return 0;
}