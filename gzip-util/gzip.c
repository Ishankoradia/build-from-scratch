#include <stdio.h>
#include <stdint.h>
#include "checksum.h"

// Gzip specification - https://datatracker.ietf.org/doc/html/rfc1952

// Huffman code paper - https://www.ias.ac.in/article/fulltext/reso/011/02/0091-0099

void write_header_bytes(FILE *out) {
    // Header bytes for gzip format
    unsigned char header[10];

    // to identify the file as being in gzip format
    header[0] = 0x1f;
    header[1] = 0x8b;

    // compression method
    header[2] = 0x08;

    // flags - set to 0 for no optional fields
    header[3] = 0x00;

    // modified timestamp (4 bytes)
    header[4] = 0x00;
    header[5] = 0x00;
    header[6] = 0x00;
    header[7] = 0x00;

    // extra flags
    header[8] = 0x00;

    // operating system
    header[9] = 0x03;

    fwrite(header, 1, sizeof(header), out);
}

void write_trailer_bytes(FILE *out, unsigned char *input_bytes, size_t bytes_read, unsigned int crc_table[]) {
    // Trailer bytes for gzip format
    unsigned char trailer[8];

    uint32_t crc = compute_crc32(bytes_read, input_bytes);

    trailer[0] = crc & 0xff;
    trailer[1] = (crc >> 8) & 0xff;
    trailer[2] = (crc >> 16) & 0xff;
    trailer[3] = (crc >> 24) & 0xff;

    // ISIZE - size of the original input file
    trailer[4] = bytes_read & 0xff;
    trailer[5] = (bytes_read >> 8) & 0xff;
    trailer[6] = (bytes_read >> 16) & 0xff;
    trailer[7] = (bytes_read >> 24) & 0xff;

    fwrite(trailer, 1, sizeof(trailer), out);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <file-name>\n", argv[0]);
        return 1;
    }

    printf("File to compress: %s\n", argv[1]);

    FILE *in = fopen(argv[1], "rb");
    if (in == NULL) {
        printf("Cannot open file: %s\n", argv[1]);
        return 1;
    }
    unsigned char buffer[65535];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in);
    fclose(in);

    // Create output filename
    char outname[256];
    snprintf(outname, sizeof(outname), "%s.gz", argv[1]);

    // Write output file
    FILE *out = fopen(outname, "wb");
    if (out == NULL) {
        printf("Cannot create output file: %s\n", outname);
        return 1;
    }
    
    // precompute crc table
    unsigned int crc_table[256];
    precompute_crc_table(crc_table);

    unsigned char block_header[5];
    block_header[0] = 0x03;                          // BFINAL=1, BTYPE=01 (fixed Huffman)
    block_header[1] = bytes_read & 0xff;              // length low byte
    block_header[2] = (bytes_read >> 8) & 0xff;       // length high byte
    block_header[3] = ~bytes_read & 0xff;             // one's complement of length low
    block_header[4] = (~bytes_read >> 8) & 0xff;      // one's complement of length high

    write_header_bytes(out);
    fwrite(block_header, 1, sizeof(block_header), out);
    fwrite(buffer, 1, bytes_read, out);
    write_trailer_bytes(out, buffer, bytes_read, crc_table);
    fclose(out);

    return 0;
}