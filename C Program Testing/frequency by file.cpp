#include <stdio.h>
#define SIZE 10

int main()
{
	FILE *fp = fopen("grades.txt", "r");

	if (fp == NULL)
		printf("The file couldn't be opened.\n");
	else
	{
		int grd, count[SIZE] = { 0 }, i = 0, maxInd;

		while (fscanf(fp, "%d", &grd) != EOF)
			count[grd-1]++;
		
		fclose(fp);

		//display the grades and their frequencies
		printf("GRADE		FREQUENCY\n");
		for (int j = 0; j < SIZE; j++)
			printf("%4d %15d\n", j+1, count[j]);

		maxInd = 0;
		for (int i = 1; i < SIZE; i++)
		{
			if (count[i] > count[maxInd])
				maxInd = i;
		}

		printf("Most of the students(%d students) have got the grade %d\n", count[maxInd], maxInd+1);

	}
	return(0);
}
