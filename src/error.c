#include "error.h"
#include "helper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error(const char* msg){
	printf("Error: %s\n", msg );
	exit(1);
}