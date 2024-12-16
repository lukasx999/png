#ifndef _PNG_H
#define _PNG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>



typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t  bit_depth;
    uint8_t  color_type;
    uint8_t  compression_method;
    uint8_t  filter_method;
    uint8_t  interlace_method;
} ImageHeader;


typedef struct {
    // TODO: not working correctly
    /* zlib data */
    uint8_t  compression_method;
    uint8_t  additional_flags;
    size_t   block_count;
    uint8_t *data_blocks;
    uint32_t check_value;
} ImageData;

typedef enum {
    CHUNK_TYPE_INVALID,
    CHUNK_TYPE_HEADER,
    CHUNK_TYPE_DATA,
    CHUNK_TYPE_END,
} ChunkType;



typedef struct Chunks Chunks;

typedef struct {
    ChunkType type;
    union {
        ImageHeader chunk_imageheader;
        ImageData   chunk_imagedata;
    };
    uint8_t *binary_data; // raw binary data of the chunk, for reconstruction purposes
    size_t  size;          // size of the entire chunk
} Chunk;


extern void      print_bytes       (const uint8_t *bytes, size_t len);
extern bool      check_signature   (FILE *f);
extern Chunk     parse_chunk       (FILE *f);
extern void      reconstruct_png   (const char *filename, Chunks *chunks);



struct Chunks {
    size_t _capacity;
    size_t size;
    Chunk *items;
};

extern Chunks chunks_new    (void);
extern void   chunks_append (Chunks *chunks, Chunk new);
extern void   chunks_destroy(Chunks *chunks);









#endif // _PNG_H
