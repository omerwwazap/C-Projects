#include <stdio.h>

void menu()
{
	printf("      MENU\n1. Draw a single line\n2. Draw a rectangle\n3. EXIT\n");
}

void dispLine(char symbol, int n)
{
	for (int i = 1; i <= n; i++)
		printf("%c", symbol);

	printf("\n");
}

void dispRectangle(int side1, int side2, char symbol)
{
	for (int i = 1; i <= side1; i++)
		dispLine(symbol, side2);
}

void main()
{
	int	choice, side1, side2, n;
	char symbol;

	do
	{
		menu();

		do
		{
			printf("Enter your choice: ");
			scanf("%d", &choice);
		} while (choice < 1 || choice >3);

		printf("\nEnter a symbol: ");
		scanf(" %c", &symbol);

		switch (choice)
		{
		case 1:
			printf("Enter the number of symbols to be displayed: ");
			scanf("%d", &n);

			dispLine(symbol, n);
			break;
		case 2:
			printf("Enter the side1: ");
			scanf("%d", &side1);

			printf("Enter the side2: ");
			scanf("%d", &side2);

			dispRectangle(side1, side2, symbol);
		}

	} while (choice != 3);
}
