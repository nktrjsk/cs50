#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        printf("Input file cannot be opened\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input_file);

    // Use check_format to ensure WAV format
    // TODO #4
    if (!check_format(header))
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output_file = fopen(argv[2], "wb");

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output_file);
    if (output_file == NULL)
    {
        printf("Output file cannot be opened\n");
        return 1;
    }

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = (long)get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    fseek(input_file, 0L, SEEK_END);
    long blocks = (ftell(input_file) - 44) / block_size;
    fseek(input_file, -block_size, SEEK_END);

    BYTE block[block_size];
    for (long i = blocks; i != 0; i--)
    {
        fread(&block, block_size, 1, input_file);
        fwrite(&block, block_size, 1, output_file);
        fseek(input_file, -block_size * 2, SEEK_CUR);
    }

    fclose(input_file);
    fclose(output_file);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    char format[] = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != format[i])
        {
            return 0;
        }
    }

    return 1;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    return header.numChannels * (header.bitsPerSample / 8);
}