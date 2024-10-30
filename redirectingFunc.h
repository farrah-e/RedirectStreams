#include <stdio.h>
#include <stdlib.h>

void redirectCommand(const char *inputFile, const char *cmd, const char *outputFile);
void forkIt(const char *inputFile, const char *cmd, const char *outputFile);
char* find_command_path(const char *command_path);