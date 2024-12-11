#include <string.h>
#include <stdlib.h>

#include "./png.h"



static void print_bytes(const char *bytes, size_t len) {
    for (size_t i=0; i < len-1; ++i) {
        printf("%x, ", bytes[i]);
    }
    printf("%x\n", bytes[len-1]);
}



bool check_signature(FILE *f) {
    const size_t sig_len  = 8;
    const char *signature = "\x89\x50\x4e\x47\xd\xa\x1a\xa";

    char bytes[8] = { 0 };
    size_t bytes_read = fread(bytes, sizeof(char), sig_len, f);

    if (bytes_read < sig_len) {
        return false;
    }

    int result = memcmp(bytes, signature, 8);
    return !result;

}


uint32_t parse_chunk_length(FILE *f) {

    const size_t block_len = 4;
    char bytes[4] = { 0 };
    fread(bytes, sizeof(char), block_len, f);

    // Bytes are stored on disk as big endian
    uint32_t length = bytes[3] +
        (bytes[2] << 8) +
        (bytes[1] << 16) +
        (bytes[0] << 24);

    return length;

}



// typedef enum {
// } ChunkType;

typedef struct {
    bool ancillary;
} ChunkType;

void parse_chunk_type(FILE *f) {

    const size_t block_len = 4;
    char bytes[4] = { 0 };
    fread(bytes, sizeof(char), block_len, f);

    printf("%.4s\n", bytes);
    // strncmp(bytes, "IHDR", 4);
    // strncmp(bytes, "IDAT", 4);
    // strncmp(bytes, "IEND", 4);

    bool ancillary    = bytes[0] & (1 << 4);
    bool private      = bytes[1] & (1 << 4);
    bool reserved     = bytes[2] & (1 << 4);
    bool safe_to_copy = bytes[3] & (1 << 4);

}

void parse_chunk_data(FILE *f, uint32_t length) {
    fseek(f, length, SEEK_CUR);
}

void parse_chunk_crc(FILE *f) {
    // Skipping for now
    fseek(f, 4, SEEK_CUR);
}



// typedef struct {
//     ChunkType kind;
//     size_t data_len;
//     void *data;
// } Chunk;

void parse_chunk(FILE *f) {

    uint32_t data_length = parse_chunk_length(f);
    printf("length: %d\n", data_length);
    parse_chunk_type(f);
    parse_chunk_data(f, data_length);
    parse_chunk_crc(f);

}
