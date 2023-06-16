#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SEG printf("%s:%d\n", __FILE__, __LINE__);

void error(int errorCode);
int headerInfo(char* filename);

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
        if (argc <= 2){
            error(1);
        }
        headerInfo(argv[2]);
        return 0;
    }


}

void error(int errorCode){
    switch (errorCode)
    {
    case 1:
        printf("Not enough arguments. Exiting.\n");
        exit(0);
        break;
    
    case 2:
        printf("Couldn't open file.\n");
        exit(0);
        break;

    default:
        printf("Error lol\n");
        exit(0);
        break;
    }
}

int headerInfo(char* filename){
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
        error(2);
    
    // file type
    char* fileType = malloc(3*sizeof(char));
    fileType[0] = (char) fgetc(bitmap);
    fileType[1] = (char) fgetc(bitmap);
    fileType[2] = '\0';
    printf("%s\n", fileType);

    // file size
    fseek(bitmap, 0x02, SEEK_SET);
    int fileSize = fgetc(bitmap);
    fileSize += (fgetc(bitmap) << 8);
    fileSize += (fgetc(bitmap) << 16);
    fileSize += (fgetc(bitmap) << 24);
    printf("The image is %d bytes large.\n", fileSize);

    // header size
    fseek(bitmap, 0x0E, SEEK_SET);
    int headerSize = 0;
    headerSize = fgetc(bitmap);
    printf("The header is %d bytes long.\n", headerSize);

    // if the file is not "BM" or its' (declared) header is >40B, I won't handle it.
    // 40B is the length of BITMAPINFOHEADER
    if(headerSize != 40 || strcmp(fileType, "BM") != 0)
        error(0);

    // width
    fseek(bitmap, 0x12, SEEK_SET);
    int width = 0;
    width = fgetc(bitmap);
    width += (fgetc(bitmap) << 8);
    printf("The bitmap is %d pixels wide.\n", width);

    // height
    fseek(bitmap, 0x16, SEEK_SET);
    int height = 0;
    height = fgetc(bitmap);
    height += (fgetc(bitmap) << 8);
    printf("The bitmap is %d pixels tall.\n", height);
    fseek(bitmap, 0x2, SEEK_CUR);

    fclose(bitmap);
    return 0;
}