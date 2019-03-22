#include<stdio.h>
#include<math.h>

int main(void)
{
	double account, tax, interest, total;
	printf("Enter account value: ");
	scanf("%lf", &account);

	//caculation
	tax = account * 0.12;
	interest = account * 0.15;

	if (account <= 100000)
	{
		total = account - tax + interest;
		printf("In one year your money will be: %0.2f\n", total);
	}

	else
	{
		interest = account * 0.17;
		total = account - tax + interest;
		printf("In one year your money will be: %0.2f\n", total);
	}

	return 0;
}

