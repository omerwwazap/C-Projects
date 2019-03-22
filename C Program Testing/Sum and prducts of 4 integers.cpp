/* Write a modular C program that gets a four-digit whole number from the user, displays the menu, 
and calculates the sum or product of the digits of that number according to the user’s choice on the menu. */
#include <stdio.h>

//display a menu on the screen
char menu()
{
	char choice;

	printf("\n          MENU\n");
	printf("**************************\n");
	printf("S) Sum of the digits\n");
	printf("P) Product of the digits\n");
	printf("E) Exit\n");

	do
	{
		printf("Enter your choice: ");
		scanf(" %c", &choice);
	} while (choice != 'S' && choice != 's' && choice != 'P' && choice != 'p' && choice != 'E' && choice != 'e');

	return choice;
}

//finds and returns the sum of the digits of the given number
int sumOfDigits(int number)
{
	int	dig1, dig2, dig3, dig4;

	dig1 = number / 1000;
	dig2 = number % 1000 / 100;
	dig3 = number % 100 / 10;
	dig4 = number % 10;

	return(dig1 + dig2 + dig3 + dig4);
}

//finds and returns the product of the digits of the given number
int prodOfDigits(int number)
{
	int	dig1, dig2, dig3, dig4;

	dig1 = number / 1000;
	dig2 = number % 1000 / 100;
	dig3 = number % 100 / 10;
	dig4 = number % 10;

	return(dig1 * dig2 * dig3 * dig4);
}

int main(void)
{
	int	num;

	do 
	{
		printf("Please enter a four-digit number: ");
		scanf("%d", &num);
	} while (num < 1000 || num > 9999);
	
	char choice;

	do
	{
		choice = menu();
		
		switch (choice)
		{
		case 'S':
		case 's':
			printf("\nThe sum of the digits: %d\n", sumOfDigits(num));
			break;
		case 'P':
		case 'p':
			printf("\nThe product of the digits: %d\n", prodOfDigits(num));
			break;
		default:
			printf("Goodbye.\n");
			break;
		}

	} while (choice != 'E' && choice != 'e');

	return 0;
}
