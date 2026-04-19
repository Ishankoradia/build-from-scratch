#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

typedef struct {
    unsigned int code;
    int bit_length;
} HuffmanCode;

typedef struct {
    unsigned int code;
    int bit_length;
    unsigned int extra_bit_value;
    int extra_bit_length;
} HuffmanDistanceCode;

HuffmanCode huffman_encode(int value);

HuffmanDistanceCode huffman_encode_distance(int value);

HuffmanDistanceCode huffman_encode_length(int value);

#endif