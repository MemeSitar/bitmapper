#ifndef _ERROR_H
#define _ERROR_H

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

#endif