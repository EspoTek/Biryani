#include "configurationfilehandler.h"
#include <stdio.h>

configurationFileHandler::configurationFileHandler()
{

}

int configurationFileHandler::loadFile(char *fname){
    printf("\nconfigurationFileHandler::loadFile(%.1024s)\n", fname);

    //Open the file
    FILE* fptr;
    fptr = fopen(fname, "r");
    if(fptr == NULL){
        printf("\nError: file %s could not be opened", fname);
        return 1;
    }


    //Close the file
    fclose(fptr);
    return 0;
}
