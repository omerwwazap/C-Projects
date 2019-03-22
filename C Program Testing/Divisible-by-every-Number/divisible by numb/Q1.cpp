#include<stdio.h>

int isdivisibe(int m, int n)
{
	if (m%n == 0)
		return 1;
	else
		return 0;
}

int countfir(int arr[], int size, int numb)
{
	int count = 0, i;
	for (i = 0; i < size; i++)
	{
		if (isdivisibe(arr[i], numb))
			count++;
	}
	return(count);
}

int main(void)
{
	int number[5], num, count, i;
	printf("Enter number to check:");
	scanf("%d", &num);

	printf("enter 5 number:");
	for (i = 0; i < 6; i++)
		scanf("%d", &number[i]);


	count = countfir(number, 5, num);
	printf("%d\n", count);

	return 0;
}