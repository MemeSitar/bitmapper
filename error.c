#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"

void error(ErrorEnum errorType){
    switch (errorType)
    {
    case MISSING_ARGUMENTS:
        printf("Not enough arguments. Exiting.\n");
        exit(0);
        break;
    
    case FILE_OPEN_ERR:
        printf("Couldn't open file.\n");
        exit(1);
        break;

    case UNSUPPORTED_FILE_FORMAT:
        printf("Unsupported file format.\n");
        exit(1);
        break;

    case FILESIZE_ERR:
        printf("Filesize sanity check failed: unexpected filesize.\n");
        exit(1);
        break;

    case TOO_MANY_ARGS:
        printf("Too many arguments were provided, some were ignored.\n");
        return;
        break;

    case GENERAL_ERROR:
    default:
        printf("Error lol\n");
        exit(1);
        break;
    }
}