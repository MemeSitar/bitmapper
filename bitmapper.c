#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitmapper.h"

#define SEG printf("%s:%d\n", __FILE__, __LINE__);

int main(int argc, char* argv[]){
    
    // initial check
    if (argc <= 1 || strcmp(argv[1], "-h") == 0){
        printf("%s\n%s\n%s\n%s\n%s\n%s\n",
            "Not enough arguments, baka!!",
            "The following switches are possible: (do note, the switches must be directly after the program!)",
            "-h for help (prints this)",
            "-hi [file] for header info",
            "-s [file] to strip the file's header",
            "-a [original, file] to add original's header to");
        return 0;
    }

    if (strcmp(argv[1], "-hi") == 0){
        if (argc <= 2)
            error(MISSING_ARGUMENTS);
        headerInfo(argv[2]);
        return 0;
    }


}

void error(ErrorEnum errorType){
    switch (errorType)
    {
    case MISSING_ARGUMENTS:
        printf("Not enough arguments. Exiting.\n");
        exit(0);
        break;
    
    case FILE_OPEN_ERR:
        printf("Couldn't open file.\n");
        exit(0);
        break;

    case UNSUPPORTED_FILE_FORMAT:
        printf("Unsupported file format.\n");
        exit(0);
        break;

    default:
        printf("Error lol\n");
        exit(0);
        break;
    }
}

Image* headerInfo(char* filename){
    /*
    * the header should be of the type BITMAPINFOHEADER, as specified on
    * https://en.wikipedia.org/wiki/BMP_file_format
    * a "header" is split into two parts: 
    * - the first 14 bytes are the same for all bitmap formats
    * - and the following X bytes, where BITMAPINFOHEADER has 40B.
    * meaning the *entire* header is 14B + 40B = 54B
    * That's also the part that will be removed.
    */

    FILE* bitmap = fopen(filename, "rb");
    if(bitmap == NULL)
        error(FILE_OPEN_ERR);
    
    // file type
    char fileType[3];
    fgets(fileType, 3, bitmap);

    // file size
    fseek(bitmap, 0x02, SEEK_SET);
    size_t fileSize = fgetc(bitmap);
    fileSize += (fgetc(bitmap) << 8);
    fileSize += (fgetc(bitmap) << 16);
    fileSize += (fgetc(bitmap) << 24);

    // header size
    fseek(bitmap, 0x0E, SEEK_SET);
    int headerSize = 0;
    headerSize = fgetc(bitmap);

    #ifdef __DEBUG__
    printf("%s\n", fileType);
    printf("The image is %ld bytes large.\n", fileSize);
    printf("The header is %d bytes long.\n", headerSize);
    #endif

    // if the file is not "BM" or its' header is >40B, I won't handle it.
    // 40B is the length of BITMAPINFOHEADER
    if(headerSize != 40 || strcmp(fileType, "BM") != 0)
        error(UNSUPPORTED_FILE_FORMAT);

    // width
    fseek(bitmap, 0x12, SEEK_SET);
    int width = 0;
    width = fgetc(bitmap);
    width += (fgetc(bitmap) << 8);

    // height
    fseek(bitmap, 0x16, SEEK_SET);
    int height = 0;
    height = fgetc(bitmap);
    height += (fgetc(bitmap) << 8);

    #ifdef __DEBUG__
    printf("The bitmap is %d pixels wide.\n", width);
    printf("The bitmap is %d pixels tall.\n", height);
    #endif

    fclose(bitmap);

    Image* metadata = malloc(sizeof(Image));
    metadata->size = fileSize;
    metadata->height = height;
    metadata->width = width;
    
    return metadata;
}