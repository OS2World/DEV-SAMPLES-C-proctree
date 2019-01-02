/* proctree.c
**
** Non-Copyright (c) 1990, Christopher Laforet.
** Released to the public domain.
**
** Revision History:
**
** $Header$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include "proctree.h"



struct node **nodes = NULL;
short cur_nodes = 0;
short max_nodes = 0;
short level = 0;
short current = 0;


int main(void)
	{
	fprintf(stderr,"PROCTREE (v %d.%02d 0f %s): View Process Tree Structure\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Non-Copyright 1990, Chris Laforet.  Released into the Public Domain.\n\n");

	fprintf(stderr,"Loading procedure information...\n\n");
	get_procedures();
	printf("PID 0000\n");
	print_tree(0,0);
	return 0;
	}


