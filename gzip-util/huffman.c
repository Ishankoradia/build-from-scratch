#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "huffman.h"


// using the fixed huffman code trees defined in spec
// https://datatracker.ietf.org/doc/html/rfc1951

HuffmanCode huffman_encode(int value) {
    HuffmanCode result;
    if (value <= 143) {
        result.code = 48 + value;
        result.bit_length = 8;
    } else if (value <= 255) {
        result.code = 400 + (value - 144);
        result.bit_length = 9;
    } else if (value <= 279) {
        result.code = 0 + (value - 256);
        result.bit_length = 7;
    } else if (value <= 287) {
        result.code = 192 + (value - 280);
        result.bit_length = 8;
    } else {
        // ... inside huffman_encode:
        printf("Invalid huffman value: %d\n", value);
        exit(1);
    }
    return result;
};

HuffmanDistanceCode huffman_encode_distance(int value) {
    HuffmanDistanceCode result;

        // base distance for each code
    int base[] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129,
                    193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097,
                    6145, 8193, 12289, 16385, 24577};

    // extra bits for each code
    int extra[] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

    for (int i = 29; i >= 0; i--) {
        if (value >= base[i]) {
            result.code = i;
            result.bit_length = 5;
            result.extra_bit_value = value - base[i];
            result.extra_bit_length = extra[i];
            break;
        }
    }

    return result;
};

HuffmanDistanceCode huffman_encode_length(int length) {
    HuffmanDistanceCode result;

    int base[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                  35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};

    int extra[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                   3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};

    for (int i = 28; i >= 0; i--) {
        if (length >= base[i]) {
            HuffmanCode hc = huffman_encode(257 + i);
            result.code = hc.code;
            result.bit_length = hc.bit_length;
            result.extra_bit_value = length - base[i];
            result.extra_bit_length = extra[i];
            break;
        }
    }

    return result;
};