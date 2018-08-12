#include "error.h"
#include "helper.h"
#include <stdio.h>
#include <string.h>

FILE* fp = NULL;

int init(){
	fp = fopen("error.txt", "w");
	if(fp == NULL){
		return 0;
	}else
	return 1;
}

void error(const char* msg){
	fprintf(fp, "Error: %s\n", msg );
}