#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "lexer.h"
#include "data_types.h"
#include "parser.h"
#include "code.h"

char* infile;
char* outfile;

int main(int argc, char* argv[]){

    parseArg(argc, argv);
    
    Buffer file = loadFromFile(infile);

    TokenNode* tf = tokenizingFile(file);  

    Line* parsedFile = parsing(tf);
  
    programCode(parsedFile,outfile);

}
