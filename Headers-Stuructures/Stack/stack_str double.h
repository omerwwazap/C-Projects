//
// Stack Implementation, CTIS 152
//
#define STACK_SIZE  100

typedef  char  SType;

SType  STACK_EMPTY[STACK_SIZE] = "";

typedef struct
{	int	top;
	SType data[STACK_SIZE][STACK_SIZE];
} stack_t;

//Functions in this file...
void initialize_s (stack_t *s);
int is_empty_s (stack_t *s);
int is_full_s (stack_t *s);
void push (stack_t *s, SType *item);
SType* pop (stack_t *s);

//------------------------------------------------------------------------------

void initialize_s (stack_t *s)
{
	s->top = -1;
}

//------------------------------------------------------------------------------

int is_empty_s (stack_t *s)
{
	if (s->top == -1)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------

int is_full_s (stack_t *s)
{
	if (s->top == STACK_SIZE - 1)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------

void push (stack_t *s, SType *item)
{
	if (is_full_s (s))
		printf("Error: Stack is full!\n");
	else
	{
		(s->top)++;
		strcpy(s->data[s->top] , item);
	}
}

//------------------------------------------------------------------------------

SType* pop (stack_t *s)
{
	SType item[STACK_SIZE];
	if (is_empty_s(s))
	{	printf("Error: Stack is empty!\n");
		strcpy(item , STACK_EMPTY);	
	}
	else
	{	strcpy(item , s->data[s->top]);
		(s->top)--;
	}		    	
	return (item);
}
char *reverse(char *sentence)
{
	int i = 0, start = 0, len;
	char word[SIZE] = "";

	stack_t s;
	initialize_s(&s);

	while (i < strlen(sentence))
	{
		//to find a word or the last word of the sentence
		while (sentence[i] != ' ' && sentence[i] != '\0')
			i++;

		strncpy(word, sentence + start, i - start);
		word[i - start] = '\0';

		push(&s, word);

		i++; //ignore blank character
		start = i;
	}

	strcpy(sentence, " ");

	while (!is_empty_s(&s))
	{
		strcat(sentence, pop(&s));
		strcat(sentence, " ");
	}

	return sentence;
}