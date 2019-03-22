#include<stdio.h>
#define SIZE 10
int main(void)
{
	char sentence[SIZE];
	int num_a = 0, lenght = 0, i;

	printf("enter the 10 letter word(end whit a full stop)");
	sentence[lenght] = getchar();
	while (sentence[lenght] != '.')
	{
		if (sentence[lenght] == 'a' || sentence[lenght] == 'A')
		{
			num_a++;
			lenght++;
			sentence[lenght] = getchar();
		}
		printf("num of A/a is %d", num_a);
		printf("revers of sentense:");
		for (i = SIZE; i >= 0; i--)
			putchar(sentence[i]);

	}


	return 0;
}