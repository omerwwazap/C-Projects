
#include <stdio.h>

void main()
{
	FILE *fp = fopen("Q1.txt", "r");

	if (fp == NULL)
		printf("The file Q1.txt couldn't be opened.\n");
	else
	{
		int num, sum = 0;
		
		
		printf("Numbers read from the file were: ");

		//continue to read using while(... != EOF){...} as there is an unkown amount of items
		while(fscanf(fp, "%d", &num) != EOF)
		{
			//display the value that was read on screen
			printf("%d ", num);
			
			//sum it up
			sum += num;
		}

		

		//or this***********************
		//int status = fscanf(fp, "%d", &num);

		//while (status != EOF)
		//{
		//	printf("%d ", num);
		//	sum += num;
		//	status = fscanf(fp, "%d", &num);
		//}
		//************************************
		

		//close file pointer as soon as it is done being processed
		fclose(fp);

		//finally display the summation of all numbers here, after whence all reading operation is complete to ensure all nums were summed up
		printf("\nThe summation of these numbers were %d\n", sum);
	}
}
