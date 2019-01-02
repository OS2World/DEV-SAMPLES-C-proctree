/* p_print.c
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
#include "proctree.h"



short check_parent(USHORT parent,USHORT node)
	{
	short count;

	for (count = (node + 1); count < cur_nodes; count++)
		{
		if (nodes[count]->ppid == parent)
			return 1;
		}
	return 0;
	}



short check_line(USHORT parent)
	{
	USHORT count;

	for (count = 0; count < cur_nodes; count++)
		{
		if (!nodes[count]->printed && nodes[count]->ppid == parent)
			return 1;
		}
	return 0;
	}



USHORT get_parent(USHORT pid)
	{
	short count;

	for (count = 0; count < cur_nodes; count++)
		{
		if (nodes[count]->pid == pid)
			return nodes[count]->ppid;
		}
	printf("\nError: Unknown parent for PID %04x\n",pid);
	exit(1);
	}


void print_tree(USHORT parent,short level)
	{
	USHORT count;
	USHORT kount;
	USHORT counter;
	USHORT parent_check;

	for (count = 0; count < cur_nodes; count++)
		{
		if (nodes[count]->ppid == parent && !nodes[count]->printed)
			{
			if (level)
				{
				if (check_line(0))
					printf(" \xb3   ");
				else
					printf("     ");
				for (kount = 1; kount < level; kount++)
					{
					parent_check = nodes[count]->ppid;
					for (counter = 0; counter < level - kount; counter++)
						parent_check = get_parent(parent_check);
					if (check_line(parent_check))
						printf(" \xb3   ");
					else
						printf("     ");
					}
				}
			if (level >= 0)
				{
				if (check_parent(parent,count))
					printf(" \xc3\xc4\xc4\xc4");
				else
					printf(" \xc0\xc4\xc4\xc4");
				}
			printf(" %s (PID=%04x Th=%u)\n",nodes[count]->name,nodes[count]->pid,nodes[count]->threads);
			nodes[count]->printed = 1;
			print_tree(nodes[count]->pid,level + 1);
			}
		}
	}



#if 0
void print_tree(void)
	{
	int count;

	for (count = 0; count < cur_nodes; count++)
		{
		if (nodes[count]->name)
			printf("PID=%04x PPID=%04x Name=%s (%u)\n",nodes[count]->pid,nodes[count]->ppid,nodes[count]->name,nodes[count]->threads);
		else 
			printf("PID=%04x PPID=%04x Name=\"PID%04x\" (%u)\n",nodes[count]->pid,nodes[count]->ppid,nodes[count]->pid,nodes[count]->threads);
		}
	}
#endif
