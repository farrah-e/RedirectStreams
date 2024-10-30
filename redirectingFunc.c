#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "redirectingFunc.h"

void redirectCommand(const char *inputFile, const char *cmd, const char *outputFile) {
    printf("Input File: %s\n", inputFile);
    printf("Command: %s\n", cmd);
    printf("OutputFile: %s\n", outputFile);
    printf("Please look at the output file for the result\n");
    pid_t childpid = fork();
    if (childpid == 0) forkIt(inputFile, cmd, outputFile);
    else wait(NULL);
}
void forkIt(const char *inputFile, const char *cmd, const char *outputFile) {
    int input, output;

    // Handle input and output redirection
    if (strcmp(inputFile,"-") == 0 || (outputFile, "-") == 0) {
        input = STDIN_FILENO; 
        output = STDOUT_FILENO; 
    } else {
        input = open(inputFile, O_RDONLY);
        output = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (input < 0) {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
        if (output < 0) {
            perror("Failed to open output file");
            close(input);
            exit(EXIT_FAILURE);
        }
    }

    dup2(input, STDIN_FILENO);
    dup2(output, STDOUT_FILENO);
    close(input);
    close(output);
    system(cmd);
    char *args[100];
    char *cmd_copy = strdup(cmd);
    char *token = strtok(cmd_copy, " ");
    int i = 0;

    while (token != NULL && i < 100 - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
     char *command_path = find_command_path(args[0]);
    if (command_path) {
        execve(command_path, args, NULL);  // Use execve with the environment
        perror("Execve failed");
        free(command_path);
    } else {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }

    free(cmd_copy);
    exit(EXIT_FAILURE);
}


char* find_command_path(const char *cmd) {
    char *path = getenv("PATH");
    if (!path) return NULL;

    char *token;
    char *path_copy = strdup(path);
    char *full_path = malloc(10000);  // Allocate enough space for the path

    token = strtok(path_copy, ":");
    while (token != NULL) {
        snprintf(full_path, 1000, "%s/%s", token, cmd);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;  // Return the found command path
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    free(full_path);
    return NULL;  // Command not found
}