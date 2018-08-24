#include "helper.h"
#include "data_types.h"
#include "tables.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <argp.h>
#include <getopt.h>

//parser cmd line

extern char* outfile;
extern char *infile;

void parseArg(int argc, char* argv[]){
	int c;
	//program call: asm -o outfilename infilename
	while((c = getopt(argc, argv, "o:")) != -1){
	    switch(c){
	        case 'o': infile = optarg;
	        break;
            case '?':
                if (optopt == 'i' || optopt == 'o')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                exit(1);
            default:
                abort();
	    }
	}
	if(optind == argc)
		error("Must enter input file");
	infile = argv[optind];
	if(++optind < argc)
		error("Just one input file is allowed");

}

Buffer loadFromFile(const char* filename){
	Buffer buff ;
	buff.buff = NULL;
	buff.buffersize = 0;

	if(!filename) {
		error("file does not exist: L32,helper.c ");
		return buff;
	}
	
	FILE* file = fopen(filename, "r");
	if(!file) {
		error("File have not opened properly: L38,helper.c");
		return buff;
	}
	long size;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	char* filebuff = (char*) malloc(size + 1);
	if(!filebuff)
	{
			fclose(file);
			error("allocating problemL L50, helper.c");
			return buff;
	}
	
	fread(filebuff, size, sizeof(char),file);
	filebuff[size]='\0';
	fclose(file);
	
	buff.buff = filebuff;
	buff.buffersize = size;
	return buff;	
}

void freeBuff(Buffer filebuff){
	free(filebuff.buff);
	filebuff.buffersize = 0;
}

int is_num_decimal(const char *number){
	
	if(!number) return -1;

	while(*number){
		if(!isdigit(*number)){
			return 0;
		}
		++number;
	}	
	return 1;
}

int is_substr(const char *str, const char *substr){

	if(!str || !substr){
		return -1;
	}

	if(strlen(substr) > strlen(str))
		return 0;

	while(*substr){
		if(*substr++ != *str++)
			return 0;
	}
	return 1;
}


//help to search tables


const Instruction* search_for_instruction(char* ins){
	int i = 0;

	while(i <= size_table_ins){
		if(strcmp(ins, table_of_instructions[i].ins_name)==0)
			return table_of_instructions + i;
		else
			i++;
	}
	return NULL;
}
const Directive* search_for_directive(char* dir){
	int i = 0;


	while(i <= size_table_dir){
		if(strcmp(dir, table_of_directives[i].dir)==0)
			return table_of_directives + i;
		else
			i++;
	}
	return NULL;
}

const char* search_for_register(char* reg){

	int i = 0;

	while(i < size_table_reg){
		if(strcmp(reg, table_of_registers[i])==0)
			return table_of_registers[i];
		else
			i++;
	}
	return NULL;
}
