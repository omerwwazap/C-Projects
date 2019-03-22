#include <stdio.h>
#include <math.h>
int main()
{
	int n, i;
	double m;
	double result = 1;
	printf("Please enter two numbers m and n\n");
	scanf(" %lf%d", &m, &n);
	if (n < 0)
	{
		for (i = 1; i <= -n; i++)
		{
			result *= 1 / m;
		}
	}
	else if (n >= 0)
	{
		for (i = 1; i <= n; i++)
		{
			result *= m;
		}
	}



	printf("The result is %0.2f\n", result);
	return 0;