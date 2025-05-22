// file_loader.c
#include "file_loader.h"
#include <stdio.h>
#include <stdlib.h>

char *load_file_to_string(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("ERROR: Failed to open file");
        return NULL;
    }

    // Get file size
    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("ERROR: Failed to seek to end of file");
        fclose(file);
        return NULL;
    }

    long length = ftell(file);
    if (length < 0)
    {
        perror("ERROR: Failed to get file size");
        fclose(file);
        return NULL;
    }
    rewind(file);

    // Allocate buffer
    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        perror("ERROR: Failed to allocate memory for file buffer");
        fclose(file);
        return NULL;
    }

    // Read file contents
    size_t read_size = fread(buffer, 1, length, file);
    if (length < 0 || read_size != (size_t)length)
    {
        fprintf(stderr, "ERROR: Failed to read the entire file. Expected %ld bytes, read %zu bytes.\n", length, read_size);
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);

    return buffer;
}
