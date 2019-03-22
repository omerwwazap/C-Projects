#include<stdio.h>
#include <math.h>
int main()
{
	int i;
	int a = 0;
	int b = 0;
	double total, num;
	double result = 0, resultNeg, resultPos;
	printf("Enter how many numbers you want to get the average of\n");
	scanf(" %lf", &total);
	for (i = 1; i <= total; i++)
	{
		printf("Please enter a number\n");
		scanf(" %lf", &num);

		if (num < 0)
		{

			result += num;
			resultNeg += num;
			a++;
		}
		else if (num >= 0)
		{
			result += num;
			resultPos += num;
			b++;
		}
		else;
	}
	printf("The overall average is %0.2f\n", result / total);
	printf("The positive numbers average is %0.2f\n", resultPos / b);
	printf("The negative numbers average is %0.2f\n", resultNeg / a);
	return 0;
}