/* p_proc.c
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



extern USHORT APIENTRY DosQProcStatus(PVOID pBuf,USHORT cbBuf);
UCHAR bBuf[0x8000];




int ppid_compare(struct node **arg1,struct node **arg2)
	{
	return stricmp((*arg1)->name,(*arg2)->name);
	}



void get_procedures(void)
	{
	USHORT type;
	USHORT selector;
	USHORT offset;
	USHORT count;
	USHORT kount;
	USHORT temp;
	UCHAR buffer[256];
	UCHAR *cptr;
	UCHAR *ptr;

	if (DosQProcStatus(bBuf,sizeof(bBuf)))
		{
		printf("Error: Unable to get process information\n");
		exit(1);
		}
	ptr = bBuf;
	selector = SELECTOROF(ptr);
	while ((type = *(USHORT *)ptr) != 0xffff)
		{
		ptr += 2;
		offset = *(USHORT *)ptr;
		ptr += 2;

		if (!type)		/* relationship */
			{
			if (cur_nodes >= max_nodes)
				{
				if (!(nodes = realloc(nodes,(max_nodes += 50) * sizeof(struct node *))))
					{
					printf("Error: Out of memory!\n");
					exit(1);
					}
				}
			if (!(nodes[cur_nodes] = calloc(1,sizeof(struct node))))
				{
				printf("Error: Out of memory!\n");
				exit(1);
				}
			nodes[cur_nodes]->pid = *(USHORT *)ptr;
			ptr += 2;
			nodes[cur_nodes]->ppid = *(USHORT *)ptr;
			ptr += 2;
			ptr += 2;
			nodes[cur_nodes]->signiture = *(USHORT *)ptr;
			++cur_nodes;
			}
		else if (type == 1)		/* thread information */
			{
			ptr += 2;
			temp = *(USHORT *)ptr;
			for (count = 0; count < cur_nodes; count++)
				{
				if (nodes[count]->pid == temp)
					{
					++nodes[count]->threads;
					break;
					}
				}
			}
		else if (type == 2)		/* process name */
			{
			temp = *(USHORT *)ptr;
			for (count = 0; count < cur_nodes; count++)
				{
				if (nodes[count]->signiture == temp)
					{
					ptr += 2;
					temp = *(USHORT *)ptr;
					ptr += 2;
					ptr += 2;
					ptr += 2;

					for (kount = 0; kount < temp; kount++)
						ptr += 2;
            
					cptr = buffer;
					while (*cptr++ = *ptr)
						++ptr;

					if (!(nodes[count]->name = calloc(strlen(buffer) + 1,sizeof(UCHAR))))
						{
						printf("Error: Out of memory!\n");
						exit(1);
						}
					strcpy(nodes[count]->name,buffer);
					break;
					}
				}
			}

		ptr = MAKEP(selector,offset);
		}

	for (count = 0; count < cur_nodes; count++)		/* look for any nodes without names */
		{
		if (!nodes[count]->name)
			{
			sprintf(buffer,"Unknown",nodes[count]->pid);
			if (!(nodes[count]->name = calloc(strlen(buffer) + 1,sizeof(UCHAR))))
				{
				printf("Error: Out of memory!\n");
				exit(1);
				}
			strcpy(nodes[count]->name,buffer);
			}
		}

	for (count = 0; count < cur_nodes; count++)		/* look for any nodes without living parents */
		{
		if (nodes[count]->ppid)		/* process 0 is special */
			{
			for (kount = 0; kount < cur_nodes; kount++)
				{
				if (nodes[count]->ppid == nodes[kount]->pid)
					break;
				}
			if (kount >= cur_nodes)		/* parent died, invent a "fake" parental node */
				{
				if (cur_nodes >= max_nodes)
					{
					if (!(nodes = realloc(nodes,(max_nodes += 50) * sizeof(struct node *))))
						{
						printf("Error: Out of memory!\n");
						exit(1);
						}
					}
				if (!(nodes[cur_nodes] = calloc(1,sizeof(struct node))))
					{
					printf("Error: Out of memory!\n");
					exit(1);
					}
				nodes[cur_nodes]->pid = nodes[count]->ppid;
				nodes[cur_nodes]->ppid = 0;		/* since we do not KNOW the parent - the parent is considered the startup */
				sprintf(buffer,"Pid %04x (Killed)",nodes[cur_nodes]->pid);
				if (!(nodes[cur_nodes]->name = calloc(strlen(buffer) + 1,sizeof(UCHAR))))
					{
					printf("Error: Out of memory!\n");
					exit(1);
					}
				strcpy(nodes[cur_nodes]->name,buffer);
				++cur_nodes;
				}
			}
		}
	qsort(nodes,cur_nodes,sizeof(struct node *),ppid_compare);
	}
