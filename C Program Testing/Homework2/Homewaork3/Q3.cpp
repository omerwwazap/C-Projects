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
	total = account - tax + interest;


	if (account > 100000)
	{
		interest = account * 0.17;
		printf("In one year your money will be: %0.2f\n", total);
	}
	else
	
		printf("In one year your money will be: %0.2f\n", total);
	



	return 0;
}

