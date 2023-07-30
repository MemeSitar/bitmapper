#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitmapper.h"

#define SEG printf("%s:%d\n", __FILE__, __LINE__);

int main(int argc, char* argv[]){
    
    // initial check
    if (argc <= 1 || strcmp(argv[1], "-h") == 0){
        printf("%s\n\n%s\n%s\n%s\n%s\n%s\n\n%s\n%s\n\n",
            "Not enough arguments, baka!!",
            "The following switches are possible:",
            "-h for help (prints this)",
            "-hi [file] for header info",
            "-s [file, (out)] to strip the file's header",
            "-a [original, file] to add original's header to file",
            "NOTE: the switches must be directly after the program!",
            "NOTE: filenames may NOT be larger than 100 characters.");
        return 0;
    }

    // arglength check
    for(int i = 0; i < argc; i++){
        if(strlen(argv[i]) > 100)
            error(GENERAL_ERROR);
    }

    // headerinfo
    if (strcmp(argv[1], "-hi") == 0){
        if (argc <= 2)
            error(MISSING_ARGUMENTS);
        Image* metadata = headerInfo(argv[2]);
        headerPrinter(metadata);
        return 0;
    }

    //strip
    if (strcmp(argv[1], "-s") == 0){
        if (argc <= 2)
            error(MISSING_ARGUMENTS);

        // soft error.
        if (argc > 4)
            error(TOO_MANY_ARGS);
        
        // pretty much just to check the file checks out.
        Image* metadata = headerInfo(argv[2]);

        if (argc == 3){ // three arguments, only the input file
            // need to append to input file string .stripped

        } else  if (argc == 4) { // four arguments, also the output file
            // use output file name.
            
        }
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
    size_t apparentFileSize = filesize(bitmap);

    // header size
    fseek(bitmap, 0x0E, SEEK_SET);
    int headerSize = 0;
    headerSize = fgetc(bitmap);

    #ifdef __DEBUG__
    printf("File type: %s\n", fileType);
    printf("HEADER: The image is %ld bytes large.\n", fileSize);
    printf("ACTUAL: The image is %ld bytes large.\n", apparentFileSize);
    printf("The header is %d bytes long.\n", headerSize);
    #endif

    // header reporting wrong file size!!!!
    if(fileSize != apparentFileSize)
        error(FILESIZE_ERR);

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

    if(height*width*3 != fileSize - 54)
        error(FILESIZE_ERR);

    fclose(bitmap);

    Image* metadata = malloc(sizeof(Image));
    metadata->size = fileSize;
    metadata->height = height;
    metadata->width = width;
    
    return metadata;
}

size_t filesize(FILE* file){
    // Gets the actual size of the file (in B).
    // TODO maybe use fstat() in the future?
    size_t savedPosition = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t endPosition = ftell(file);
    fseek(file, savedPosition, SEEK_SET);
    return endPosition;
}

void headerPrinter(Image* metadata){
    //one more sanity check.
    if(metadata->height*metadata->width*3 != metadata->size - 54)
        error(FILESIZE_ERR);
    
    printf("The image is %ldB large\n", metadata->size);
    printf("The dimensions are: %dx%d\n", metadata->width, metadata->height);
    printf("Sanity checks out.\n");
}

void headerStripper(Image* metadata, char* inputName, char* outputName){

}