#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "list.h"
#include "common.h"

void fillMatrixRandom(int **matrix, int row, int columns, int min, int max)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			matrix[i][j] = (rand() % (max - min + 1)) + min;
		}
	}
}

int **createMatrix(int rows, int columns)
{
	int **matrix;
	matrix = (int **) malloc(rows * sizeof(int *));	// Assign memory for rows
	if(matrix == NULL)
	{
		errorMessage("Error to assign memory");
	}
	
	for(int i = 0; i < rows; i++)
	{
		matrix[i] = createList(columns);
		if(matrix[i] == NULL)
		{
			errorMessage("Error to assign memory");
		}
	}
	
	return matrix;
}

void freeMatrix(int **table, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		free(table[i]);
	}
	free(table);
}
