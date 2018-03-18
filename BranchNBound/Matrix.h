#pragma once
#include "Arc.h"

class Matrix
{
private:
	double** matrix;
	int n_rows; //Number of rows
	int n_columns; // Number of columns

	double find_row_min_elem(int);
	double find_column_min_elem(int);
	double find_row_min_elem(int, int, int);
	double find_column_min_elem(int, int, int);
	void get_idxs(int, int, int*, int*);
	Matrix(int, int);

public:
	int rows();
	int columns();
	Matrix(int);
	~Matrix();
	Matrix* clone();
	Matrix* create_cutted_copy(int, int);
	void matrix_reduction(double*);
	Arc* get_next_arc(double*);
	double get_matrix_min_elem(Arc*);
	double get_matrix_min_path(Arc***);
	void set_value(int, int, double);
	void print_matrix();
	void creat_test_matrix();
	void create_asyn_matrix(double, int, double);
	void create_symmetric();
	void add_delta(double, int);
};

