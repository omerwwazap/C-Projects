#include<stdio.h>

int main(void)
{
	int number,		//store the number
		sum; 	//total sum of the digits

	printf("Enter a positive integer (non-positive to exit): ");
	scanf("%d", &number);

	while (number > 0)
	{
		do
		{
			sum = 0;//each time make sum 0 in order to calculate the next number's sum of digits

			while (number != 0)
			{
				//calculate the sum of the digits
				sum += number % 10;

				//then get rid of that final digit
				number /= 10;
			}

			//display the sum of the digits of the number
			printf("Sum of digits = %d\n", sum);

			//new number will be the sum, as otherwise the number remains 0 and only 1 iteration takes place, try commenting this line out to try
			number = sum;

		} while (sum >= 10);//continue to loop until the sum of digits is less than 9 (means only have 1 digit)

		printf("\nEnter a positive integer (non-positive to exit): ");
		scanf("%d", &number);

	}

	return 0;
}
