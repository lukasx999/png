#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "./png.h"

#include <zlib.h>





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


    const uint8_t *hidden_message = (uint8_t*) "\x0\x0\x0\x3\x0\x0\x16\x0\x66\x6f\x6f\x0\x0\x0\x0";


    Chunks chunks = chunks_new();

    Chunk chunk = { 0 };
    while (chunk.type != CHUNK_TYPE_END) {
        chunk = parse_chunk(f);
        chunks_append(&chunks, chunk);
    }

    reconstruct_png("new.png", &chunks);

    chunks_destroy(&chunks);




    fclose(f);

    return EXIT_SUCCESS;

}
