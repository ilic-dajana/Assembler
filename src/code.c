#include "code.h"
#include "firstpass.h"
#include "secondpass.h"
#include "parser.h"
#include "data_types.h"
#include "error.h"
#include "helper.h"

#include "stdio.h"
#include "stdlib.h"


void programCode(Line* parserOutput, const char* filename){
    if(!parserOutput || !filename)
        error("NULL pointer exception (L15, code.c)");
    firstPass(parserOutput);
    secondPass(parserOutput);

    FILE* outputFile = NULL;

    if(!outputFile)
        error("File not exist(L22, code.c)");



    fclose(outputFile);
    deleteRelocationTable(&reltab, 10);
    deleteSymbolTable(&tab);
}