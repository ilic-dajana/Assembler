#ifndef _HELPER_H_
#define _HELPER_H_

#include "data_types.h"

//File options

Buffer loadFromFile(const char *filename);
void freeBuff(Buffer buff);

//Checks

int is_num_decimal(const char *number);
int is_substr(const char *str, const char *substr);



#endif
