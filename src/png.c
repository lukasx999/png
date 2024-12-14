#include <string.h>
#include <stdlib.h>

#include "./png.h"



static void print_bytes(const char *bytes, size_t len) {
    for (size_t i=0; i < len-1; ++i) {
        printf("%x, ", bytes[i]);
    }
    printf("%x\n", bytes[len-1]);
}



// Important: use uint8_t instead of char, because of signedness
static uint64_t number_from_bytes(const uint8_t *bytes, size_t len) {

    uint64_t number = 0;

    // Data in PNG is stored as big endian
    for (size_t i=0; i < len; ++i) {
        size_t inv = (len-1) - i;
        number += bytes[inv] << (i * 8);
    }

    return number;

}






bool check_signature(FILE *f) {
    const size_t sig_len  = 8;
    const uint8_t *signature = (uint8_t*) "\x89\x50\x4e\x47\xd\xa\x1a\xa";

    uint8_t bytes[8] = { 0 };
    size_t bytes_read = fread(bytes, sizeof(uint8_t), sig_len, f);

    if (bytes_read < sig_len) {
        return false;
    }

    int result = memcmp(bytes, signature, 8);
    return !result;

}

uint32_t parse_chunk_length(FILE *f) {

    uint8_t bytes[4] = { 0 };
    fread(bytes, sizeof(uint8_t), 4, f);

    uint32_t length = number_from_bytes(bytes, 4);
    return length;

}

ChunkType parse_chunk_type(FILE *f) {

    char bytes[4] = { 0 };
    fread(bytes, sizeof(char), 4, f);

    printf("type: %.4s\n", bytes);

    if (!strncmp(bytes, "IHDR", 4)) {
        return CHUNK_TYPE_HEADER;

    } else if (!strncmp(bytes, "IDAT", 4)) {
        return CHUNK_TYPE_DATA;

    } else if (!strncmp(bytes, "IEND", 4)) {
        return CHUNK_TYPE_END;

    } else {
        return CHUNK_TYPE_INVALID;
    }

}

Chunk parse_chunk_data(FILE *f, ChunkType type, uint32_t length) {

    uint8_t *bytes = malloc(length * sizeof(char));
    fread(bytes, sizeof(uint8_t), length, f);

    Chunk chunk = { 0 };
    chunk.type = type;

    switch (type) {

        case CHUNK_TYPE_HEADER: {
            ImageHeader *header = &chunk.chunk_imageheader;

            uint8_t width_buf[4] = { 0 };
            memcpy(width_buf, bytes, 4);
            uint32_t width = number_from_bytes(width_buf, 4);
            header->width = width;

            uint8_t height_buf[4] = { 0 };
            memcpy(height_buf, bytes+4, 4);
            uint32_t height = number_from_bytes(height_buf, 4);
            header->height = height;

            header->bit_depth          = bytes[5];
            header->color_type         = bytes[6];
            header->compression_method = bytes[7];
            header->filter_method      = bytes[8];
            header->interlace_method   = bytes[9];

        } break;

        case CHUNK_TYPE_DATA: {
            ImageData *data = &chunk.chunk_imagedata;
            data->compression_method = bytes[0];
            data->additional_flags   = bytes[1];

            data->block_count = length - 6;
            data->data_blocks = malloc(data->block_count * sizeof(uint8_t));
            memcpy(data->data_blocks, bytes+2, data->block_count);

            data->check_value = (uint32_t) number_from_bytes(bytes+(length-4), 4);
        } break;

        case CHUNK_TYPE_END:
        case CHUNK_TYPE_INVALID: break;
    }

    free(bytes);
    return chunk;

}

void parse_chunk_crc(FILE *f) {
    // Skipping for now
    fseek(f, 4, SEEK_CUR);
}



Chunk parse_chunk(FILE *f) {

    uint32_t data_length = parse_chunk_length(f);
    ChunkType type = parse_chunk_type(f);
    Chunk chunk = parse_chunk_data(f, type, data_length);
    parse_chunk_crc(f);

    return chunk;

}
