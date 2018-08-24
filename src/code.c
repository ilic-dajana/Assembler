#include "code.h"
#include "firstpass.h"
#include "secondpass.h"
#include "parser.h"
#include "data_types.h"
#include "error.h"
#include "helper.h"

#include "stdio.h"
#include "stdlib.h"


void programCode(Line* parsedOutput, char* filename){
    if(!parsedOutput || !filename)
        error("NULL pointer exception (L15, code.c)");
    firstPass(parsedOutput);
    secondPass(parsedOutput);

    FILE* outputFile = NULL;
    outputFile = fopen(filename, "w");

    if(!outputFile)
        error("File not exist(L22, code.c)");

    writeSymTabToFile(tab, outputFile);
    writeRelTabToFIle(outputFile, secN, &reltabls[0], tab);

    fclose(outputFile);
    //deleteRelocationTable(reltab, 10);
    deleteSymbolTable(tab);
}