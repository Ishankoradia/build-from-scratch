#include <stddef.h>
#include <stdint.h>

static uint32_t crc_table[256];
static int table_ready = 0;
static uint32_t polynomial = 0xEDB88320L;

// CRC (checksum) - very nicely explained here - https://www.youtube.com/watch?v=izG7qT0EpBw&list=PLLFa1kmhcdi04H5otHAyABjxgDz7vhD72

void precompute_crc_table() {
    // Implementation for precomputing CRC table
    int n, k;
    unsigned long c;
    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = (c >> 1) ^ polynomial;
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
}

uint32_t compute_crc32(size_t bytes_read, unsigned char *input_bytes) {
    
    if (!table_ready) {
        precompute_crc_table();
        table_ready = 1;
    }

    uint32_t crc = 0xffffffff; // 1111......32 bits
    for(size_t i = 0; i < bytes_read; i++) {
        crc = crc_table[(crc ^ input_bytes[i]) & 0xff] ^ (crc >> 8);
    }
    // flip one more time
    crc = crc ^ 0xffffffff;

    return crc;
}