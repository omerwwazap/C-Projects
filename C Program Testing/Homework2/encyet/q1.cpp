#include <stdio.h>
#include <time.h>
#include <stdlib.h>
/*
Encryption program
gets senctenc from file
encrpts 
than
prints on the console

*/
void encrypt(char sentence[], int size, char encrypt[])
{
	srand(time(NULL));
	int i, value = rand() % 5 + 1;
	for (i = 0; i<size; i++)
	{
		if (sentence[i] != ' ')
			encrypt[i] = value + sentence[i];
		else
			encrypt[i] = sentence[i];
	}
}
void writefile(char encrypt[], int size)
{
	int i;
	for (i = size - 1; i >= 0; i--)
		printf("%c", encrypt[i]);
	printf("\n");
}
int main()
{
	FILE *inp;
	inp = fopen("hi.txt", "r");
	int status, count = 0;
	char sentence[200], encrypte[200];
	if (inp == NULL)
		printf("fak u");
	else
	{
		status = fscanf(inp, "%c", &sentence[count]);
		while (status != EOF)
		{
			while (sentence[count] != '\n' && status != EOF)
			{
				count++;
				status = fscanf(inp, "%c", &sentence[count]);
			}
			encrypt(sentence, count, encrypte);
			writefile(encrypte, count);
			count = 0;
			status = fscanf(inp, "%c", &sentence[count]);

		}
	}
	fclose(inp);
	return 0;
}