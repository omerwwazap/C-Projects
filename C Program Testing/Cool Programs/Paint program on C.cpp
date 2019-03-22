#include<stdio.h>
#define ROW 60
#define COL 29


void fillArray(int row, int col, char arr[][COL])
{
	for (col = 0; col < COL; col++)
	{
		for (row = 0; row < ROW; row++)
			arr[row][col] = { ' ' };
	}
}

void dispArray(int row, int col, char arr[][COL])
{
	for (col = 0; col < COL; col++)
	{
		for (row = 0; row < ROW; row++)
			printf("%c ", arr[row][col]);
	}
}

void dispFrame(int row, int col, char arr[][COL])
{
	col = 0;
	row = 0;
	for (col; col < COL; col++)
	{
		arr[0][col] = { '|' };
	}
	for (row; row < ROW; row++)
	{
		arr[row][0] = { '_' };
	}
	for (col = 1; col < COL; col++)
	{
		arr[ROW - 1][col] = { '|' };
	}
	for (row = 1; row < ROW - 1; row++)
	{
		arr[row][COL - 1] = { '_' };
	}
}

int main()
{
	char arr[ROW][COL], input, pen = ' ', penchar = 'M';
	int    col, row, posrow = 20, poscol = 8, flag = 0;

	fillArray(ROW, COL, arr);
	arr[posrow][poscol] = { 'I' };
	dispFrame(ROW, COL, arr);

	printf(" Welcome to the CMD Sketch.\n");
	printf("----------------------------\n");
	printf("Controls are \"w,a,s,d\".\n");
	printf("Press \"q\" for pencil tool.\n");
	printf("Press \"c\" to change pencil.\n");
	printf("Press \"e\" for erase tool.\n");
	printf("Press \"m\" for move tool.\n");
	printf("Press \"k\" to save the sketch.\n");
	printf("Press \"h\" for help!\n");
	printf("Press any key to start.\n");
	
	scanf("%c", &input);

	dispArray(ROW, COL, arr);

	do
	{
		scanf(" %c", &input);

		if (input == 'q')
		{
			flag = 0;
			pen = penchar;
		}
		else if (input == 'e')
		{
			flag = 0;
			pen = ' ';
		}
		else if (input == 'm')
			flag = 1;
		else if (input == 'c')
		{
			printf("\nEnter a character to draw with: ");
			scanf(" %c", &penchar);
			pen = penchar;
			printf("The character \"%c\" selected.\n", penchar);
		}
		else if (input == 'h')
		{
			printf("\nControls are \"w,a,s,d\".\nPress \"q\" for pencil tool.\nPress \"c\" to change pencil.\nPress \"e\" for erase tool.\nPress \"m\" for move tool.\nPress \"k\" to save the sketch.\nPress any key to continue.\n");
			scanf(" %c", &input);
			input = 'm';
		}
		else if (input == 'k')
		{
			arr[posrow][poscol] = { pen };
			FILE *outptr = fopen("cmd_sketch.txt", "w");
			for (col = 0; col < COL; col++)
			{
				for (row = 0; row < ROW; row++)
					fprintf(outptr, "%c ", arr[row][col]);
				fprintf(outptr, "\n");
			}
			printf("File has been saved.\nPress any key to continue.\n");
			scanf(" %c", &input);
			input = 'm';
			fclose(outptr);
			arr[posrow][poscol] = { 'I' };

		}


		if (input == 'w')
		{
			arr[posrow][poscol--] = { pen };
			if (flag == 1)
				pen = arr[posrow][poscol];
			arr[posrow][poscol] = { 'I' };
		}
		else if (input == 'd')
		{
			arr[posrow++][poscol] = { pen };
			if (flag == 1)
				pen = arr[posrow][poscol];
			arr[posrow][poscol] = { 'I' };
		}
		else if (input == 'a')
		{
			arr[posrow--][poscol] = { pen };
			if (flag == 1)
				pen = arr[posrow][poscol];
			arr[posrow][poscol] = { 'I' };
		}
		else if (input == 's')
		{
			arr[posrow][poscol++] = { pen };
			if (flag == 1)
				pen = arr[posrow][poscol];
			arr[posrow][poscol] = { 'I' };
		}
		dispArray(ROW, COL, arr);
		dispFrame(ROW, COL, arr);

	} while (input != 'exit');

	return 0;
}
