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


    while (true) {
        Chunk chunk = parse_chunk(f);

        switch (chunk.type) {

            case CHUNK_TYPE_INVALID: {
            } break;

            case CHUNK_TYPE_HEADER:  {
                puts("header");

                uint8_t comp_meth = chunk.chunk_imageheader.compression_method;
                assert(comp_meth == 0);

            } break;

            case CHUNK_TYPE_DATA: {
                ImageData *data = &chunk.chunk_imagedata;

                printf("Compression method/flags code: %u\n", data->compression_method);
                printf("Additional flags/check bits: %u\n", data->additional_flags);

                // for (size_t i=0; i < data->block_count; ++i) {
                //     data->data_blocks[i];
                // }

                free(data->data_blocks);
                printf("Check value: %d\n", data->check_value);

                puts("data");
            } break;

            case CHUNK_TYPE_END: {
                puts("end");
            } break;

        }

        if (chunk.type == CHUNK_TYPE_END) {
            break;
        }
    }






    fclose(f);

    return EXIT_SUCCESS;

}
