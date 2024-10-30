#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "redirectingFunc.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <program>\n", argv[0]);
        return 1;
    }
    redirectCommand(argv[1], argv[2], argv[3]);
    return 0;
}