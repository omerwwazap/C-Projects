
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main()
{
	FILE *fp = fopen("Q2.txt", "r");

	if (fp == NULL)
		printf("The file Q2.txt couldn't be opened.\n");
	else
	{
		//declare these variables only if the necessary file was successfully opened, doing otherwise is unnecessary declarations; 1 file pointer for each file to be written
		FILE *multi = fopen("multiplies.txt", "w"), *nonMulti = fopen("nonMultiplies.txt", "w");

		//num variable for the values from the file to be stored and processed one by one through reading
		int num, randomNum;

		srand(time(NULL));
		randomNum = rand() % 5 + 1;

		printf("The random number was %d.\n", randomNum);

		//continue to read using while(... != EOF){...} as there is an unkown amount of items
		while(fscanf(fp, "%d", &num) != EOF)
		{
			//write to one file if negative
			if (num % randomNum == 0)
				fprintf(multi, "%d ", num);
			//write to the other one if positive
			else
				fprintf(nonMulti, "%d ", num);
		}

		//close all file pointers
		fclose(fp);
		fclose(nonMulti);
		fclose(multi);

		printf("The files were created successfully.\n");
	}
}
