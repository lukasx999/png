#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "./png.h"






int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];
    FILE *f = fopen(filename, "r");

    bool is_png = check_signature(f);
    if (!is_png) {
        fprintf(stderr, "Error: File is not a PNG\n");
        exit(1);
    }


    while (!feof(f)) {
        parse_chunk(f);
    }






    fclose(f);

    return EXIT_SUCCESS;

}
