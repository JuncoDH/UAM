/**
 * @file ejercicio_shell.c
 * @author Junco de las Heras junco.heras@estudiante.uam.es Marta Vaquerizo marta.vaquerizo@estudiante.uam.es
 * Grupo: 2201
 * Fecha: 16-02-2020
 * Descripcion: Ejercicio en el que tienes que crear una shell de modo que ejecute
 * los comandos que lea por stdin mediante la creacion de un hijo
 * */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFF 1024

int main(int argc, char *argv[])
{
    char *line;
    char **args;
    char *toks;
    pid_t pid;
    int n_args, i;
    int status;

    //Es la linea que leemos de stdin
    line = malloc(BUFF * sizeof(char));
    if (!line)
    {
        perror("no hay memoria para line");
        exit(EXIT_FAILURE);
    }
    //Es el prompt para leer line
    if (fprintf(stdout, ":$ ") <= 0)
    {
        exit(EXIT_FAILURE);
    }
    //Es el bucle hasta que se lea EOF
    while (fgets(line, BUFF, stdin) != NULL)
    {
        //n_args cuenta el numero de argumentos para crear un array con ese tamaño
        n_args = 0;
        for (i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ' ')
            {
                n_args++;
            }
        }
        n_args++;

        args = malloc((n_args + 1) * sizeof(char *));
        if (!args)
        {
            perror("no hay memoria para args");
            exit(EXIT_FAILURE);
        }

        //en args[i] guarda la i-ésima palabra, en args[n_args] guarda NULL
        toks = strtok(line, " \n");
        args[0] = toks;
        for (i = 1; i < n_args; i++)
        {
            toks = strtok(NULL, " \n");
            args[i] = toks;
        }
        args[n_args] = NULL;

        //hace un fork, el hijo ejecuta el comando y el padre vuelve al bucle
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            execvp(args[0], args);
            perror("execvp:");
        }

        //Espera al hijo y recoge su status de salida
        if (waitpid(pid, &status, 0) == -1)
        {
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status))
        {
            if (fprintf(stderr, "Exit with value %d\n", WEXITSTATUS(status)) <= 0)
            {
                exit(EXIT_FAILURE);
            }
        }
        else if (WIFSIGNALED(status))
        {
            if (fprintf(stderr, "Terminated by signal %d\n", WTERMSIG(status)) <= 0)
            {
                exit(EXIT_FAILURE);
            }
        }

        free(args);

        if (fprintf(stdout, ":$ ") < 0)
        {
            exit(EXIT_FAILURE);
        }
    }
    free(line);

    exit(EXIT_SUCCESS);
}
