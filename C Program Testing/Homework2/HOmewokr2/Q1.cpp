#include<stdio.h>

int main(void)
{
	double x, y, cac;

	printf("Enter the number that is gping to be diveded: ");
	scanf("%lf",&x);
	printf("Enter the number to dived it: ");
	scanf("%lf",&y);

	cac = x / y;

	if (y == 0)
	
		printf("Invalid input\n");
	
	else
		printf("The result is : %0.2f\n", cac);


	return 0;
}