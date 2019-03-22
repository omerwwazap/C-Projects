#include <stdio.h>

void main()
{
	FILE *fp = fopen("Q3cd.txt", "r");

	if (fp == NULL)
		printf("The file couldn't be opened.\n");
	else
	{
		int arr[5], i, min, max;
		

		for (i = 0; i < 5; i++)
			fscanf(fp, "%d", &arr[i]);

		//stacked assignment, kullanayim dedim, 1 kere de olsa gorsunler hic yapmadik bu donem operator stacking
		min = max = arr[0];

		printf("The array contents are: ");
		for (i = 0; i < 5; i++)
		{
			printf("%d ", arr[i]);

			if (min > arr[i])
				min = arr[i];
			else if (max < arr[i])
				max = arr[i];
		}
		
		printf("\nOut of these numbers %d was the smallest and %d was the biggest number.\n\n", min, max);
	}
}
