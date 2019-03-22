//
// Circular Queue Implementation, CTIS 152
//
#define QUEUE_SIZE  10

typedef  int  QType;

QType  QUEUE_EMPTY = -987654321;

typedef struct
{	
	int front, rear;
	QType data[QUEUE_SIZE];
	int counter;
} queue_t;

//Functions in this file...
void initialize_q (queue_t *q);
int is_empty_q (queue_t *q);
int is_full_q (queue_t *q);
void insert (queue_t *q, QType item);
QType remove (queue_t *q);

//------------------------------------------------------------------------------
void initialize_q (queue_t *q)
{
	q->front = 0;
	q->rear  = -1;
	q->counter = 0;
}

//------------------------------------------------------------------------------
int is_empty_q (queue_t *q)
{
	if (q->counter == 0)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------
int is_full_q (queue_t *q)
{
	if (q->counter == QUEUE_SIZE)
		return 1;
	return 0;
}

//------------------------------------------------------------------------------
void insert (queue_t *q, QType item)
{
	if (is_full_q (q))
		printf("Error: Queue is full!\n");
	else 
	{	
		q->rear = (q->rear + 1) % QUEUE_SIZE;   // make it circular
		q->data[q->rear] = item;
		(q->counter)++;
	}
}

//------------------------------------------------------------------------------
QType remove (queue_t *q)
{
	QType item;
	if (is_empty_q (q))
	{	
		printf("Error: Queue is empty!\n");
		item = QUEUE_EMPTY;
	}
	else
	{	
		item = q->data[q->front];	
		q->front = (q->front + 1) % QUEUE_SIZE;
		(q->counter)--;
	}
	return item;
}
void displayQueue(queue_t queue)
{
	printf("\nQueue Content\n");
	printf("----------------------\n");

	while (!is_empty_q(&queue))
		printf("%d\t", remove(&queue));
}