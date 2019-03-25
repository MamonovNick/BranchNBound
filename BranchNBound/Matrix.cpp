#include "stdafx.h"
#include "Matrix.h"
#include "Arc.h"
#include <iostream>
#include <random>
#include <ctime>

Matrix::Matrix(int n, int m) : n_rows(n), n_columns(m)
{
	matrix = new double*[n_rows];
	for (int i = 0; i < n_rows; i++)
		matrix[i] = new double[n_columns];
}

int Matrix::rows()
{
	return n_rows - 1;
}

int Matrix::columns()
{
	return n_columns - 1;
}

Matrix::Matrix(int n) : n_rows(n + 1), n_columns(n + 1)
{
	matrix = new double*[n_rows];
	for (int i = 0; i < n_rows; i++)
		matrix[i] = new double[n_columns];
}

Matrix::~Matrix()
{
	if (matrix != nullptr)
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
	if (row <= 0 && column <= 0)
		int dfg = 0;
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
			result->matrix[i + corr_i][j + corr_j] = matrix[i][j];
		}
	}

	return result;
}

double Matrix::find_row_min_elem(int row)
{
	double result = DBL_MAX;
	for (int i = 1; i < n_columns; i++)
		if (matrix[row][i] < result)
			result = matrix[row][i];
	return result;
}

double Matrix::find_row_min_elem(int row, int cur_row, int cur_column)
{
	double result = DBL_MAX;
	for (int i = 1; i < n_columns; i++)
	{
		if (row == cur_row && i == cur_column)
			continue;
		if (matrix[row][i] < result)
			result = matrix[row][i];
	}
	return result;
}

double Matrix::find_column_min_elem(int column)
{
	double result = DBL_MAX;
	for (int i = 1; i < n_rows; i++)
		if (matrix[i][column] < result)
			result = matrix[i][column];
	return result;
}

double Matrix::find_column_min_elem(int column, int cur_row, int cur_column)
{
	double result = DBL_MAX;
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
void Matrix::matrix_reduction(double* reduce_value)
{
	*reduce_value = 0;

	double* columns_min_elems = new double[n_columns - 1];
	for (int i = 0; i < n_columns - 1; i++)
		columns_min_elems[i] = DBL_MAX;

	for (int i = 1; i < n_rows; i++)
	{
		double min_elem = find_row_min_elem(i);

		*reduce_value += min_elem;

		for (int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] == DBL_MAX)
				continue;

			matrix[i][j] -= min_elem;
			if (matrix[i][j] < columns_min_elems[j - 1])
				columns_min_elems[j - 1] = matrix[i][j];
		}
	}

	for (int j = 1; j < n_columns; j++)
	{
		*reduce_value += columns_min_elems[j - 1];
		for (int i = 1; i < n_rows; i++)
		{
			if (matrix[i][j] == DBL_MAX)
				continue;

			matrix[i][j] -= columns_min_elems[j - 1];
		}
	}

	delete[] columns_min_elems;

}

//Method for finding next arc
//Cost matrix should be reduced previously
Arc* Matrix::get_next_arc(double* estimate)
{
	//fgh:
	Arc* result = new Arc();
	double max_est = std::numeric_limits<double>::lowest();

	for (int i = 1; i < n_rows; i++)
		for (int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] != 0)
				continue;
			double row_min_elem = find_row_min_elem(i, i, j);
			double column_min_elem = find_column_min_elem(j, i, j);
			double tmp_est = row_min_elem + column_min_elem;
			if (tmp_est > max_est)
			{
				result->first = static_cast<int>(matrix[i][0]);
				result->second = static_cast<int>(matrix[0][j]);
				max_est = tmp_est;
			}
		}

	*estimate = max_est;
	if (result->first < 0 && result->second < 0)
		int dfgh = 9;
	//goto fgh;
	return result;
}

double Matrix::get_matrix_min_elem(Arc* min_arc)
{
	min_arc = new Arc();
	double min_value = DBL_MAX;

	for (int i = 1; i < n_rows; i++)
		for (int j = 1; j < n_columns; j++)
		{
			if (matrix[i][j] == DBL_MAX)
				continue;
			if (matrix[i][j] < min_value)
			{
				min_value = matrix[i][j];
				min_arc->first = static_cast<int>(matrix[i][0]);
				min_arc->second = static_cast<int>(matrix[0][j]);
			}
		}

	return min_value;
}

double Matrix::get_matrix_min_path(Arc*** arcs)
{
	if (n_rows > 3)
		return -1;

	if (n_rows == 3 && n_columns == 3)
	{
		double min_val = 0;
		double diag1, diag2;

		if (matrix[1][1] == DBL_MAX || matrix[2][2] == DBL_MAX)
			diag1 = DBL_MAX;
		else
			diag1 = matrix[1][1] + matrix[2][2];
		if (matrix[2][1] == DBL_MAX || matrix[1][2] == DBL_MAX)
			diag2 = DBL_MAX;
		else
			diag2 = matrix[2][1] + matrix[1][2];

		if (diag1 == DBL_MAX && diag2 == DBL_MAX)
			return -1;

		if (diag1 < diag2)
		{
			*arcs = new Arc*[2];
			(*arcs)[0] = new Arc(static_cast<int>(matrix[1][0]), static_cast<int>(matrix[0][1]), true);
			(*arcs)[1] = new Arc(static_cast<int>(matrix[2][0]), static_cast<int>(matrix[0][2]), true);
			return diag1;
		}
		else
		{
			*arcs = new Arc*[2];
			(*arcs)[0] = new Arc(static_cast<int>(matrix[2][0]), static_cast<int>(matrix[0][1]), true);
			(*arcs)[1] = new Arc(static_cast<int>(matrix[1][0]), static_cast<int>(matrix[0][2]), true);
			return diag2;
		}
	}

	return -1;
}

void Matrix::set_value(int row, int column, double value)
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
			if (matrix[i][j] == DBL_MAX)
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
	matrix[0] = new double[7]{ 0, 1, 2, 3, 4, 5, 6 };
	matrix[1] = new double[7]{ 1, DBL_MAX,	20,	18,	12,	8 };
	matrix[2] = new double[7]{ 2, 5,	DBL_MAX,	14,	7,	11 };
	matrix[3] = new double[7]{ 3, 12,	18,	DBL_MAX,	6, 11 };
	matrix[4] = new double[7]{ 4, 11,	17,	11,	DBL_MAX,	12 };
	matrix[5] = new double[7]{ 5, 5,	5,	5,	5,	DBL_MAX };
	*/
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

	 /*
	 matrix[0] = new double[6] {0, 1, 2, 3, 4, 5};
	 matrix[1] = new double[6] {1, DBL_MAX, 25, 40, 31, 27};
	 matrix[2] = new double[6] {2, 5, DBL_MAX, 17, 30, 25};
	 matrix[3] = new double[6] {3, 19, 15, DBL_MAX, 6, 1};
	 matrix[4] = new double[6] {4, 9, 50, 24, DBL_MAX, 6};
	 matrix[5] = new double[6] {5, 22, 8, 7, 10, DBL_MAX };*/

	 /*
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
				 matrix[i][j] = DBL_MAX;
				 continue;
			 }
			 matrix[i][j] = rand() % 20 + 1;
		 }
	 }*/
}

void Matrix::create_asyn_matrix(double delta, int mode, double max_value)
{
	for (int i = 0; i < n_columns; i++)
		matrix[0][i] = i;
	for (int i = 0; i < n_rows; i++)
		matrix[i][0] = i;

	double rnd_value;
	double eps;

	switch (mode)
	{
	case 1:
		eps = delta / 2.0;
		for (int i = 1; i < n_rows; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
				{
					matrix[i][j] = DBL_MAX;
					continue;
				}
				rnd_value = rand() % 25 + 2;
				matrix[i][j] = rnd_value + eps;
				matrix[j][i] = rnd_value - eps;
			}
		break;
	case 2:
		eps = delta / 2.0;
		for (int i = 1; i < n_rows - 1; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
				{
					matrix[i][j] = DBL_MAX;
					continue;
				}
				rnd_value = rand() % 20 + 1;
				matrix[i][j] = rnd_value - eps;
				matrix[j][i] = rnd_value + eps;
			}
		break;
	case 3:
		eps = delta;
		for (int i = 1; i < n_rows - 1; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
				{
					matrix[i][j] = DBL_MAX;
					continue;
				}
				rnd_value = rand() % 20 + 1;
				matrix[i][j] = rnd_value - eps;
				matrix[j][i] = rnd_value + eps;
			}
		break;
	default:
		break;
	}
}

//void Matrix::create_symmetric()
//{
//	//Store indecies in matrix
//	for (int i = 0; i < n_columns; i++)
//		matrix[0][i] = i;
//	for (int i = 0; i < n_rows; i++)
//		matrix[i][0] = i;
//
//	for (int i = 1; i < n_rows; i++)
//		for (int j = i; j < n_columns; j++)
//		{
//			if (i == j)
//			{
//				matrix[i][j] = DBL_MAX;
//				continue;
//			}
//			//Random generation
//			double rnd_value = rand() % 35 + 6;
//			matrix[i][j] = rnd_value;
//			matrix[j][i] = rnd_value;
//		}
//}

void Matrix::create_symmetric()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);

	//Store indecies in matrix
	for (int i = 0; i < n_columns; i++)
		matrix[0][i] = i;
	for (int i = 0; i < n_rows; i++)
		matrix[i][0] = i;

	for (int i = 1; i < n_rows; i++)
		for (int j = i; j < n_columns; j++)
		{
			if (i == j)
			{
				matrix[i][j] = DBL_MAX;
				continue;
			}
			//Random generation
			double rnd_value = dis(gen);
			matrix[i][j] = rnd_value;
			matrix[j][i] = rnd_value;
		}
}

void Matrix::add_delta(double delta, int mode)
{
	double eps;
	int targetRow, targetColumn, targetRowId, targetColId;

	switch (mode)
	{
	case 1:
		eps = delta;
		for (int i = 1; i < n_rows; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
					continue;
				matrix[i][j] += eps * matrix[i][j];
				matrix[j][i] -= eps * matrix[j][i];
			}
		break;
	case 2:
		eps = delta;
		for (int i = 1; i < n_rows - 1; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
					continue;

				matrix[i][j] -= eps * matrix[i][j];
				matrix[j][i] += eps * matrix[j][i];
			}
		break;
	case 3:
		eps = delta;
		for (int i = 1; i < n_rows - 1; i++)
			for (int j = i; j < n_columns; j++)
			{
				if (i == j)
					continue;

				matrix[i][j] += eps * matrix[i][j];
			}
		break;
	case 4:
		eps = delta;
		targetRow = (n_rows + 1) / 2;
		for (int j = 1; j < n_columns; j++)
		{
			if (targetRow == j)
				continue;

			matrix[targetRow][j] += eps * matrix[targetRow][j];
		}
		break;
	case 5:
		eps = delta;
		targetColumn = (n_rows + 1) / 2;
		for (int i = 1; i < n_rows - 1; i++)
		{
			if (i == targetColumn)
				continue;

			matrix[i][targetColumn] += eps * matrix[i][targetColumn];
		}
		break;
	case 6:
		eps = delta;
		targetRowId = rand() % (n_rows - 1) + 1;
		targetColId = rand() % (n_columns - 1) + 1;
		matrix[targetRowId][targetColId] += eps * matrix[targetRowId][targetColId];

		break;
	default:
		break;
	}
}
