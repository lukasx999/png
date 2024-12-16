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


    FILE *new = fopen("new.png", "a");

    // const uint8_t *chunk = (uint8_t*) "\x0\x0\x0\x3\x0\x0\x16\x0\x66\x6f\x6f\x0\x0\x0\x0";
    // fwrite(chunk, sizeof(uint8_t), 15, new);

    fclose(new);



    while (true) {
        Chunk chunk = parse_chunk(f);

        switch (chunk.type) {

            case CHUNK_TYPE_INVALID: {
            } break;

            case CHUNK_TYPE_HEADER:  {
                uint8_t comp_meth = chunk.chunk_imageheader.compression_method;
                assert(comp_meth == 0);
            } break;

            case CHUNK_TYPE_DATA: {
                ImageData *data = &chunk.chunk_imagedata;
                free(data->data_blocks);
            } break;

            case CHUNK_TYPE_END: {
            } break;

        }

        if (chunk.type == CHUNK_TYPE_END) {
            break;
        }
    }






    fclose(f);

    return EXIT_SUCCESS;

}
