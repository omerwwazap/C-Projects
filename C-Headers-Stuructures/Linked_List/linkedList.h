#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXI 20

typedef struct node_s
{
	char data[MAXI];
	struct node_s *next;
} node_t;

node_t *getNode(void)
{
	node_t *node;
	node = (node_t *)malloc(sizeof(node_t));
	node->next = NULL;
	return (node);
}

void addAfter(node_t *p, char *item)
{
	node_t *newp;
	newp = getNode();
	strcpy(newp->data, item);
	newp->next = p->next;
	p->next = newp;
}

node_t *addBeginning(node_t *headp, char *item)
{
	node_t *newp;
	newp = getNode();
	strcpy(newp->data, item);
	newp->next = headp;
	return (newp);
}
node_t *createList(FILE *inp)
{
	node_t *headp = NULL, *p;
	char word[MAXI];

	if (fscanf(inp, " %s", word) != EOF)
	{
		headp = addBeginning(headp, word);
		p = headp;

		while (fscanf(inp, " %s", word) != EOF)
		{
			addAfter(p, word);
			p = p->next;
		}
	}
	return headp;
}

void displayList(node_t *headp)
{
	node_t *p;

	if (headp == NULL)
		printf("\nThe List is empty.\n");
	else
	{
		p = headp;
		while (p != NULL)
		{
			printf("%s\n", p->data);
			p = p->next;
		}
	}
}

void displayRec(node_t *headp, char *str)
{
	int len, lenstr;
	if (headp != NULL)
	{
		len = strlen(headp->data);
		lenstr = strlen(str);
		if (strncmp(headp->data, str, lenstr) == 0 || strncmp(headp->data + len - lenstr, str, lenstr) == 0)
			printf("%s\n", headp->data);
		displayRec(headp->next, str);
	}
}
node_t *delete_first (node_t *headp)
{	
	node_t *del;
	del = headp;
	headp = del->next;
	free (del);
	return (headp);
}

void delete_after (node_t *p)
{	
	node_t *del;
	del = p->next;
	p->next = del->next;
	free (del);
}

node_t * createList(FILE *inp)
{
	software_t cd;
	node_t *headp, *p;
	
	headp = NULL;

	
	if (fscanf(inp, "%d %d", &cd.id, &cd.copies) != EOF)
	{
		headp = addBeginning(headp, cd);
		
		p = headp;
		while (fscanf(inp, "%d %d", &cd.id, &cd.copies) != EOF)
		{
			addAfter(p, cd);
			p = p->next;
		} 
	}
	return headp;
}

node_t * deleteProduct(node_t *headp)
{
	node_t *p;
	LType item;

	//remove all cds from the beginning of the linked list which have more than 100 copies 
	while(headp->data.copies > 100)
		headp = deleteFirst(headp, &item);
		
	p = headp;
	while (p->next != NULL)
	{
		if (p->next->data.copies > 100)
			deleteAfter(p, &item);
		else
			p = p->next;
	}
	return (headp);
}
int searchForAUser(node_t *headp, char userName[])
{
	int found = 0;
	node_t *p;

	p = headp;
	while (p != NULL)
	{
		if (strcmp(p->data.userName, userName) == 0)
		{
			printf("\n%-15s %-15s %-15s ->", p->data.userName, p->data.city, p->data.lastLogin);
			found = 1;
		}

		p = p->next;
	}

	return found;
}
node_t* findLongest(node_t *headp)
{
	node_t	*longest = headp,
		*p = headp->next;

	int len, lenLongest = strlen(headp->data);

	while (p != NULL)
	{
		len = strlen(p->data);
		if (len > lenLongest)
		{
			longest = p;
			lenLongest = len;
		}
		p = p->next;
	}

	return longest;
}