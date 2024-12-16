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

typedef struct {
    ChunkType type;
    union {
        ImageHeader chunk_imageheader;
        ImageData   chunk_imagedata;
    };
} Chunk;

extern bool      check_signature   (FILE *f);
extern uint32_t  parse_chunk_length(FILE *f);
extern ChunkType parse_chunk_type  (FILE *f);
extern Chunk     parse_chunk_data  (FILE *f, ChunkType type, uint32_t length);
extern void      parse_chunk_crc   (FILE *f);
extern Chunk     parse_chunk       (FILE *f);





#endif // _PNG_H
