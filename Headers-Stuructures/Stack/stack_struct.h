#define STACK_SIZE  100

typedef  product_t  SType;

SType  STACK_EMPTY = { -9999, -999999.99, -9999 };

typedef struct
{
	int	top;
	SType data[STACK_SIZE];
} stack_t;

//Functions in this file...
void initialize_s(stack_t *s);
int is_empty_s(stack_t *s);
int is_full_s(stack_t *s);
void push(stack_t *s, SType item);
SType pop(stack_t *s);

//------------------------------------------------------------------------------

void initialize_s(stack_t *s)
{
	s->top = -1;
}

//------------------------------------------------------------------------------

int is_empty_s(stack_t *s)
{
	if (s->top == -1)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------

int is_full_s(stack_t *s)
{
	if (s->top == STACK_SIZE - 1)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------

void push(stack_t *s, SType item)
{
	if (is_full_s(s))
		printf("Error: Stack is full!\n");
	else
	{
		(s->top)++;
		s->data[s->top] = item;
	}
}

//------------------------------------------------------------------------------

SType pop(stack_t *s)
{
	SType item;
	if (is_empty_s(s))
	{
		printf("Error: Stack is empty!\n");
		item = STACK_EMPTY;
	}
	else
	{
		item = s->data[s->top];
		(s->top)--;
	}
	return (item);
}

void displayStack(stack_t s)
{
	product_t p;
	while (!is_empty_s(&s))
	{
		p = pop(&s);
		printf("%-5d%-7.2f%-5d\n", p.id, p.price, p.amount);
	}
}