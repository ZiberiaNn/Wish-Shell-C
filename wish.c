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

char *mypath[] = {"/bin/", ""};

int main(int argc, char *argv[])
{
    char *line;
    char *command_string;
    char *command_args;
    int fd;
    // Leer los comandos del usuario
    while ((line = readline("whish> ")) != NULL)
    {
        int newline_pos = strcspn(line, "\n");
        printf("%c", line[newline_pos]);
        line[newline_pos] = '\0';
        // Almacenar el comando en el historial
        add_history(line);

        command_args=line;
        command_string = strtok_r(command_args, " ", &command_args);
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
                    myargs[1] = strdup(".");
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
    return 0;
}