#include <stdio.h>

//function prototypes, these make managing functions easier before having written their definitions
void divideReal(double num, double *frac, int *integral);
int roundNum(double num);

void main()
{
	double num;

	printf("Enter a real number (-999 to stop): ");
	scanf("%lf", &num);

	while (num != -999)
	{
		printf("The rounded number is %d\n\n", roundNum(num));

		printf("Enter a real number (-999 to stop): ");
		scanf("%lf", &num);
	}
}

void divideReal(double num, double * frac, int * integral)
{
	*integral = (int)num;
	printf("The integral part: %d\n", *integral);
	*frac = num - *integral;
	printf("The fractional part: %f\n", *frac);
}

int roundNum(double num)
{
	int integral;
	double frac;

	divideReal(num, &frac, &integral);

	if (frac >= 0.5)
		return integral + 1;
	return integral;
}
