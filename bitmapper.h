#ifndef _BITMAPPER_H
#define _BITMAPPER_H

typedef struct
{
    int size;
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
int headerInfo(char* filename);

#endif