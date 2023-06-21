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
    FILESIZE_ERR,
    TOO_MANY_ARGS,
    GENERAL_ERROR
} ErrorEnum;

void error(ErrorEnum errorType);
Image* headerInfo(char* filename);
size_t filesize(FILE* file);
void headerPrinter(Image* metadata);
void headerStripper(Image* metadata, char* inputName, char* outputName);

#endif