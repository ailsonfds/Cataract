#ifndef __ARGS_H__
#define __ARGS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char diag_f_name[500];
char img_f_name[500];

void decode(int argc, char* argv[]);
void print_help();

#endif /*__ARGS_H__*/