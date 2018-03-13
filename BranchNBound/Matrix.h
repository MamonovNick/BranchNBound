#pragma once
#include "Arc.h"

class Matrix
{
private:
	int** matrix;
	int n_rows; //Number of rows
	int n_columns; // Number of columns

	int find_row_min_elem(int);
	int find_column_min_elem(int);
	int find_row_min_elem(int, int, int);
	int find_column_min_elem(int, int, int);
	void get_idxs(int, int, int*, int*);
	Matrix(int, int);

public:
	int rows();
	int columns();
	Matrix(int);
	~Matrix();
	Matrix* clone();
	Matrix* create_cutted_copy(int, int);
	void matrix_reduction(int*);
	Arc* get_next_arc(int*);
	int get_matrix_min_elem(Arc*);
	int get_matrix_min_path(Arc***);
	void set_value(int, int, int);
	void print_matrix();
	void creat_test_matrix();
};

