#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

// N is the size of the 2D matrix N*N
#define N 9


//convert the matrix of sudoku into one long string
void to_str(int grid[N][N], char str[110])
{

	int a = 0;
	for(int i = 0; i < 9; i += 1)
	{
		if(i == 3 || i == 6)
		{
			str[a] = '\n';
			a += 1;
		}

		for(int j = 0; j < 9; j += 1)
		{
			if(j == 3 || j == 6)
			{
				str[a] = ' ';
				a += 1;
			}

			str[a] = (char)(grid[i][j] + '0');
			a += 1;
		}

		str[a] = '\n';
		a += 1;
	}
}


//convert the file into a grid
void to_grid(FILE* ptr, int grid[N][N])
{
	char str[81];
	char ch;

	//get every char and put these in str
	while(!feof(ptr))
	{
		ch = fgetc(ptr);
		if(ch != ' ' && ch != '\n')
		{
			strncat(str, &ch, 1);
		}
	}

	int a = 0;
	int b = 0;


	//convert the string into the grid
	for(int i = 0; i < 81; i += 1)
	{
		if(b == 9)
		{
			b = 0;
			a += 1;
		}

		if(a == 9)
			break;

		int j = 0;

		if(str[i] != '.')
			j = str[i] - '0';

		grid[a][b] = j;

		b += 1;
	}

}


/* A utility function to print grid */
void print(int arr[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d ",arr[i][j]);
		printf("\n");
	}
}

// Checks whether it will be legal
// to assign num to the
// given row, col
int isSafe(int grid[N][N], int row,
					int col, int num)
{
	// Check if we find the same num
	// in the similar row , we return 0
	for (int x = 0; x <= 8; x++)
		if (grid[row][x] == num)
			return 0;

	// Check if we find the same num in the
	// similar column , we return 0
	for (int x = 0; x <= 8; x++)
		if (grid[x][col] == num)
			return 0;

	// Check if we find the same num in the
	// particular 3*3 matrix, we return 0
	int startRow = row - row % 3,
				startCol = col - col % 3;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (grid[i + startRow][j +
						startCol] == num)
				return 0;

	return 1;
}

/* Takes a partially filled-in grid and attempts
to assign values to all unassigned locations in
such a way to meet the requirements for
Sudoku solution (non-duplication across rows,
columns, and boxes) */
int solveSudoku(int grid[N][N], int row, int col)
{
	// Check if we have reached the 8th row
	// and 9th column (0
	// indexed matrix) , we are
	// returning true to avoid
	// further backtracking
	if (row == N - 1 && col == N)
		return 1;

	// Check if column value becomes 9 ,
	// we move to next row and
	// column start from 0
	if (col == N)
	{
		row++;
		col = 0;
	}

	// Check if the current position
	// of the grid already contains
	// value >0, we iterate for next column
	if (grid[row][col] > 0)
		return solveSudoku(grid, row, col + 1);

	for (int num = 1; num <= N; num++)
	{
		// Check if it is safe to place
		// the num (1-9) in the
		// given row ,col ->we move to next column
		if (isSafe(grid, row, col, num)==1)
		{
			/* assigning the num in the
			current (row,col)
			position of the grid
			and assuming our assigned num
			in the position
			is correct	 */
			grid[row][col] = num;

			// Checking for next possibility with next
			// column
			if (solveSudoku(grid, row, col + 1)==1)
				return 1;
		}

		// Removing the assigned num ,
		// since our assumption
		// was wrong , and we go for next
		// assumption with
		// diff num value
		grid[row][col] = 0;
	}
	return 0;
}

int main(int argc, char** argv)
{
	if (argc != 2)
		errx(EXIT_FAILURE,"only one file should be traited");

	FILE* ptr = fopen(argv[1], "r");
	if ( ptr == NULL )
	{
        	printf( "Cannot open file %s\n", argv[0] );
        	exit( 0 );
	}

	int grid[N][N];
	to_grid(ptr, grid);

	fclose(ptr);

	if (solveSudoku(grid, 0, 0)==1)
	{
		char str[110];
		to_str(grid, str);

		char* name = argv[1];
		strncat(name, ".result", 7);

		ptr = fopen(name, "w");
		fprintf(ptr, str);
		fclose(ptr);
	}
	else
		printf("No solution exists");

	return 0;
	// This is code is contributed by Pradeep Mondal P
}
