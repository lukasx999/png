#ifndef _PNG_H
#define _PNG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


extern bool check_signature(FILE *f);
extern uint32_t parse_chunk_length(FILE *f);
extern void parse_chunk_type(FILE *f);
extern void parse_chunk_data(FILE *f, uint32_t length);
extern void parse_chunk_crc(FILE *f);
extern void parse_chunk(FILE *f);


#endif // _PNG_H
