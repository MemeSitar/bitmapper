#ifndef _BITMAPPER_H
#define _BITMAPPER_H

typedef struct
{
    size_t size;
    int width;
    int height;
} Image;

typedef enum
{
    MISSING_ARGUMENTS,
    FILE_OPEN_ERR,
    UNSUPPORTED_FILE_FORMAT,
    GENERAL_ERROR
} ErrorEnum;

void error(ErrorEnum errorType);
Image* headerInfo(char* filename);
size_t filesize(FILE* file);

#endif