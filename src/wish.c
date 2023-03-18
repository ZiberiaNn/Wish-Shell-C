#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "wish_utils.c"
#define MAX_SIZE 100

void printStderr();
char *mypath[] = {"/bin/", ""};

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        exit(1);
    }
    char *command_string;
    char *command_args;
    int fd;
    

    char *line;

    // Estos atributos son para el batch mode
    FILE *fp;
    size_t len = 0;
    ssize_t read;
    if (argc == 2) // Si hay 2 argumentos, significa que se entro un archivo para el batchmode
    {
        fp = fopen(argv[1], "r"); // Se intenta abrir el archivo que se entro como argumento
        if (fp == NULL)
        {
                char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
            exit(1);
        }
    }

    // Leer los comandos del usuario
    while (1)
    {
        if (argc == 1) // Se ejecuta esta linea en modo interactivo (no se ingreso un argumento para el batchmode)
        {
            line = readline("wish> "); // Se imprime wish y se recibe la entrada del usuario en line
            int newline_pos = strcspn(line, "\n");
            line[newline_pos] = '\0';
            add_history(line); // Almacenar el comando en el historial
        }
        else if (argc == 2) // Si hay 2 argumentos, significa que se entro un archivo para el batchmode
        {   
            read = getline(&line, &len, fp); // Se lee cada linea del archivo batch
            if (read == EOF){
                break;
            }
            int newline_pos = strcspn(line, "\n");
            line[newline_pos] = '\0';
        }

        command_args = line;
        command_string = strtok_r(command_args, " ", &command_args);
        if (command_string == NULL) // Si no se entra ningún comando, empieza el loop de nuevo
        {
            continue;
        }
        else if (command_args[0] == 0 || command_args[0] == 32) // Si no se entra ningún argumento al comando, se coloca argumento "."
        {
            command_args = malloc(strlen(".") + 1);
            strcpy(command_args, ".");
        }

        if (strcmp(command_string, "exit") == 0)
        {
            execute_exit(0);
        }
        else if (strcmp(command_string, "cd") == 0)
        {
            execute_cd(command_args);
        }
        else if (strcmp(command_string, "path") == 0)
        {
            execute_path();
        }
        else
        {
            fd = -1;
            char **mp = mypath;
            char specificpath[MAX_SIZE];
            while ((strcmp(*mp, "") != 0) && fd != 0)
            {
                strcpy(specificpath, *mp++);
                strncat(specificpath, command_string, strlen(command_string));
                fd = access(specificpath, X_OK);
            }
            if (fd == 0)
            {
                int subprocess = fork();
                if (subprocess < 0)
                {
                    printf("Error launching the subprocess");
                }
                else if (subprocess == 0)
                {
                    char *myargs[3];

                    myargs[0] = strdup(specificpath);
                    myargs[1] = strdup(command_args);
                    myargs[2] = NULL;
                    execvp(myargs[0], myargs);
                }
                else
                {
                    wait(NULL);
                }
            }
            else
            {
                printf("Command not found: %s\n", line);
            }
        }
    }
    free(command_args);
    free(line);
    fclose(fp);
    return 0;
}

void printStderr(){

}