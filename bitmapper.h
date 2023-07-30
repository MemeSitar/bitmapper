#include <stdlib.h>

#ifndef _BITMAPPER_H
#define _BITMAPPER_H

typedef struct
{
    size_t size;
    int width;
    int height;
} Image;

Image* headerInfo(char* filename);
size_t filesize(FILE* file);
void headerPrinter(Image* metadata);
void headerStripper(Image* metadata, char* inputName, char* outputName);

#endif