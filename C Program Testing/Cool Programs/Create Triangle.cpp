#include <stdio.h>

int main(void)
{
	int n, i, j;

	do
	{
		printf("Enter an integer (<0 to stop):");
		scanf("%d", &n);
		for (i = 0; i <= n; i++)
		{
			for (j = 0; j <= i; j++)
				printf("%d", j);
			printf("\n");
		}
		printf("\n");
	} while (n >= 0);
	return 0;
}
