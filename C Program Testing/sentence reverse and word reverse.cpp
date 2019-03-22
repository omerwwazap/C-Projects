
#include <stdio.h>

#define MAX 25

void main()
{
	FILE *psent = fopen("sentence.txt", "r");

	if (psent == NULL)
		printf("The file couldn't be opened.\n");
	else
	{
		char sentence[MAX], word[MAX];
		int i = 0, j, status;
		FILE *prev = fopen("reverse_a.txt", "w");

		//PART-A
		//Reversing the sentence

		//read the first char from the input file
		while (fscanf(psent, "%c", &sentence[i]) != EOF)
			i++;

		fclose(psent);

		//write the reversed sentence
		for (j = i - 1; j >= 0; j--)
			fprintf(prev, "%c", sentence[j]);

		fclose(prev);
		fclose(psent);
	}
}

/*
//Question-2 Part b

#include <stdio.h>

#define MAX 25

void main()
{
	FILE *psent = fopen("sentence.txt", "r");

	if (psent == NULL)
		printf("The file couldn't be opened.\n");
	else
	{
		char sentence[MAX], word[MAX];
		int i = 0, j, status;
		FILE *prev = fopen("reverse_a.txt", "w");

		//reversing word by word

		status = fscanf(psent, "%c", &word[i]);

		while (status != EOF)
		{
			while (word[i] != ' ' && status != EOF)
			{
				i++;
				status = fscanf(psent, "%c", &word[i]);
			}
			//end of a word is reached after this while

			//write its reverse
			for (j = i - 1; j >= 0; j--)
				fprintf(prev, "%c", word[j]);

			fprintf(prev, " ");
			i = 0;
			status = fscanf(psent, "%c", &word[i]);
		}
		fclose(psent);
		fclose(psent);
	}
}
*/
