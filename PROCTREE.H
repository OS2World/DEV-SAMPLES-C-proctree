/* proctree.h
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
**
** Revision History:
**
** $Header$
**
*/


#include <os2.h>


/* 1.  Constants and Macros */

#define MAJOR_VERSION			1
#define MINOR_VERSION			0


/* 2.  Structures and Unions */

struct node
	{
    USHORT pid;
	USHORT ppid;
	USHORT signiture;
	UCHAR *name;
	USHORT threads;
	USHORT printed;
	};



/* 3.  Global Variables */

extern struct node **nodes;
extern short cur_nodes;
extern short max_nodes;
extern short level;
extern short current;


/* 4.  Functions */

extern void get_procedures(void);
extern void print_tree(USHORT parent,short level);
