

#include "helper.h"
#include "data_types.h"
#include "tables.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <argp.h>

/*
		char* args_a = "INPUT OUTPUT";
		struct argp_option options[] = {
			{"input_file", 'i',0 , 0,  "input file name"},
			{"address", 'a', "ADDR", 0, "start addr"},
			{"output_file", 'o', 0, 0, "output file"}
		};
		struct argp argp = {options, parseArgs , args_a, 0, 0, 0};
		argp_parse(&argp, argc, argv, 0, 0, &argv);
}
*/

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
