#ifndef _HELPER_H_
#define _HELPER_H_

#include "data_types.h"

//File options

Buffer loadFromFile(const char *filename);
void freeBuff(Buffer buff);

//Checks

int is_num_decimal(const char *number);
int is_substr(const char *str, const char *substr);

// Table searches

const Instruction* search_for_instruction(char* ins);
const Directive* search_for_directive(char* dir);
const char* search_for_register(char* reg);

//Cmd line arguments parsing
extern char* outfile;
extern char* infile;

void parseArg(int argc, char* argv[]);
#endif
