#include "stdafx.h"
#include "Matrix.h"
#include <climits>
#include <xutility>
#include "Arc.h"
#include <iostream>
#include <random>
#include <ctime>

Matrix::Matrix(int n, int m) : n_rows(n), n_columns(m)
{
	matrix = new int*[n_rows];
	for(int i = 0; i < n_rows; i++)
		matrix[i] = new int[n_columns];
}

int Matrix::rows()
{
	return n_rows - 1;
}

int Matrix::columns()
{
	return n_columns - 1;
}

Matrix::Matrix(int n) : n_rows(n+1), n_columns(n+1)
{
	matrix = new int*[n_rows];
	for (int i = 0; i < n_rows; i++)
		matrix[i] = new int[n_columns];
}

Matrix::~Matrix()
{
	if(matrix != nullptr)
	{
		for (int i = 0; i < n_rows; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
}

Matrix* Matrix::clone()
{
	Matrix* result = new Matrix(n_rows, n_columns);
	for (int i = 0; i < n_rows; i++)
		for (int j = 0; j < n_columns; j++)
			result->matrix[i][j] = matrix[i][j];

	return result;
}

Matrix* Matrix::create_cutted_copy(int row, int column)
{
	int set_row = -1, set_column = -1;
	get_idxs(row, column, &set_row, &set_column);

	Matrix* result = new Matrix(n_rows - 1, n_columns - 1);
	int corr_i = 0;
	for (int i = 0; i < n_rows; i++)
	{
		if (i == set_row)
		{
			corr_i = -1;
			continue;
		}
		int corr_j = 0;
		for (int j = 0; j < n_columns; j++)
		{
			if (j == set_column)
			{
				corr_j = -1;
				continue;
			}
			result->matrix[i+corr_i][j+corr_j] = matrix[i][j];
		}
	}

	return result;
}

int Matrix::find_row_min_elem(int row)
{
	int result = INT_MAX;
	for (int i = 1; i < n_columns; i++)
		if (matrix[row][i] < result)
			result = matrix[row][i];
	return result;
}

int Matrix::find_row_min_elem(int row, int cur_row, int cur_column)
{
	int result = INT_MAX;
	for (int i = 1; i < n_columns; i++)
	{
		if (row == cur_row && i == cur_column)
			continue;
		if (matrix[row][i] < result)
			result = matrix[row][i];
	}
	return result;
}

int Matrix::find_column_min_elem(int column)
{
	int result = INT_MAX;
	for (int i = 1; i < n_rows; i++)
		if (matrix[i][column] < result)
			result = matrix[i][column];
	return result;
}

int Matrix::find_column_min_elem(int column, int cur_row, int cur_column)
{
	int result = INT_MAX;
	for (int i = 1; i < n_rows; i++)
	{
		if (i == cur_row && column == cur_column)
			continue;
		if (matrix[i][column] < result)
			result = matrix[i][column];
	}
	return result;
}

//This function DOESN'T create copy of matrix
void Matrix::matrix_reduction(int* reduce_value)
{
	*reduce_value = 0;

	int* columns_min_elems = new int[n_columns-1];
	for (int i = 0; i < n_columns-1; i++)
		columns_min_elems[i] = INT_MAX;

	for (int i = 1; i < n_rows; i++)
	{
		int min_elem = find_row_min_elem(i);

		*reduce_value += min_elem;

		for (int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] == INT_MAX)
				continue;

			matrix[i][j] -= min_elem;
			if (matrix[i][j] < columns_min_elems[j-1])
				columns_min_elems[j-1] = matrix[i][j];
		}
	}

	for (int j = 1; j < n_columns; j++)
	{
		*reduce_value += columns_min_elems[j-1];
		for (int i = 1; i < n_rows; i++)
		{
			if (matrix[i][j] == INT_MAX)
				continue;

			matrix[i][j] -= columns_min_elems[j-1];
		}
	}
}

//Method for finding next arc
//Cost matrix should be reduced previously
Arc* Matrix::get_next_arc(int* estimate)
{
	Arc* result = new Arc();
	int max_est = INT_MIN;

	for (int i = 1; i < n_rows; i++)	
		for (int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] != 0)
				continue;
			int row_min_elem = find_row_min_elem(i, i, j);
			int column_min_elem = find_column_min_elem(j, i, j);
			int tmp_est = row_min_elem + column_min_elem;
			if(tmp_est > max_est)
			{
				result->first = matrix[i][0];
				result->second = matrix[0][j];
				max_est = tmp_est;
			}
		}

	*estimate = max_est;
	return result;
}

int Matrix::get_matrix_min_elem(Arc* min_arc)
{
	min_arc = new Arc();
	int min_value = INT_MAX;

	for (int i = 1; i < n_rows; i++)
		for(int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] == INT_MAX)
				continue;
			if(matrix[i][j] < min_value)
			{
				min_value = matrix[i][j];
				min_arc->first = matrix[i][0];
				min_arc->second = matrix[0][j];
			}
		}

	return min_value;
}

int Matrix::get_matrix_min_path(Arc*** arcs)
{
	if (n_rows > 3)
		return -1;

	if(n_rows == 3 && n_columns == 3)
	{
		int min_val = 0;
		int diag1, diag2;

		if (matrix[1][1] == INT_MAX || matrix[2][2] == INT_MAX)
			diag1 = INT_MAX;
		else
			diag1 = matrix[1][1] + matrix[2][2];
		if (matrix[2][1] == INT_MAX || matrix[1][2] == INT_MAX)
			diag2 = INT_MAX;
		else
			diag2 = matrix[2][1] + matrix[1][2];

		if (diag1 == INT_MAX && diag2 == INT_MAX)
			return -1;

		if(diag1 < diag2)
		{
			*arcs = new Arc*[2];
			(*arcs)[0] = new Arc(matrix[1][0], matrix[0][1], true);
			(*arcs)[1] = new Arc(matrix[2][0], matrix[0][2], true);
			return diag1;
		}
		else
		{
			*arcs = new Arc*[2];
			(*arcs)[0] = new Arc(matrix[2][0], matrix[0][1], true);
			(*arcs)[1] = new Arc(matrix[1][0], matrix[0][2], true);
			return diag2;
		}
	}

	return -1;
}

void Matrix::set_value(int row, int column, int value)
{
	int set_row = -1, set_column = -1;
	get_idxs(row, column, &set_row, &set_column);
	if (set_row == -1 || set_column == -1)
		return;
	matrix[set_row][set_column] = value;
}

void Matrix::get_idxs(int row, int column, int* x, int* y)
{
	for (int i = 1; i < n_rows; i++)
		if (matrix[i][0] == row)
		{
			*x = i;
			break;
		}

	for (int j = 1; j < n_columns; j++)
		if (matrix[0][j] == column)
		{
			*y = j;
			break;
		}
}

void Matrix::print_matrix()
{
	std::cout << "======================\n";
	if (matrix == nullptr || n_rows <= 0 || n_columns <= 0)
		return;
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_columns; j++)
		{
			if (matrix[i][j] == INT_MAX)
			{
				std::cout << "inf  ";
				continue;
			}
			std::cout << matrix[i][j] << "  ";
		}
		std::cout << "\n";
	}
	std::cout << "======================\n";
}

void Matrix::creat_test_matrix()
{

	/*
	matrix[0] = new int[7]{ 0, 1, 2, 3, 4, 5, 6 };
	matrix[1] = new int[7]{ 1, INT_MAX,	26,	42,	15,	29,	25 };
	matrix[2] = new int[7]{ 2, 7,	INT_MAX,	16,	1,	30,	25 };
	matrix[3] = new int[7]{ 3, 20,	13,	INT_MAX,	35,	5,	0 };
	matrix[4] = new int[7]{ 4, 21,	16,	25,	INT_MAX,	18,	18 };
	matrix[5] = new int[7]{ 5, 12,	46,	27,	48,	INT_MAX,	5 };
	matrix[6] = new int[7]{ 6, 23,	5,	5,	9,	5,	INT_MAX };
	 */

	/*matrix[0] = new int[7]{ 0, 1, 2, 3, 4, 5, 6 };
	matrix[1] = new int[7]{ 1, INT_MAX,	7,	16,	21,	2,	17 };
	matrix[2] = new int[7]{ 2, 13, INT_MAX, 21, 15, 43, 23 };
	matrix[3] = new int[7]{ 3, 25, 3, INT_MAX, 31, 17, 9 };
	matrix[4] = new int[7]{ 4, 13, 10, 27, INT_MAX, 33, 12 };
	matrix[5] = new int[7]{ 5, 9, 2, 19, 14, INT_MAX, 51 };
	matrix[6] = new int[7]{ 6, 42, 17, 5, 9, 23, INT_MAX };*/


	/*matrix[0] = new int[6] {0, 1, 2, 3, 4, 5};
	matrix[1] = new int[6] {1, INT_MAX, 25, 40, 31, 27};
	matrix[2] = new int[6] {2, 5, INT_MAX, 17, 30, 25};
	matrix[3] = new int[6] {3, 19, 15, INT_MAX, 6, 1};
	matrix[4] = new int[6] {4, 9, 50, 24, INT_MAX, 6};
	matrix[5] = new int[6] {5, 22, 8, 7, 10, INT_MAX};*/

	srand(time(0));
	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_columns; j++)
		{
			if (j == 0)
			{
				matrix[i][j] = i;
				continue;
			}
			if (i == 0)
			{
				matrix[i][j] = j;
				continue;
			}
			if (i == j)
			{
				matrix[i][j] = INT_MAX;
				continue;
			}
			matrix[i][j] = rand() % 20 + 1;
		}
	}
}


