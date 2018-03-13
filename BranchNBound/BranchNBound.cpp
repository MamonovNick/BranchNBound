// BranchNBound.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matrix.h"
#include <climits>
#include <cstdlib>
#include "ExecuteBranchNBound.h"
#include <ctime>
#include <iostream>


int main()
{
	unsigned int start_time = clock(); // начальное время

	Matrix* matrix = new Matrix(45);
	
	//
	matrix->creat_test_matrix();
	//

	matrix->print_matrix();

	ExecuteBranchNBound* bnb = new ExecuteBranchNBound();
	bnb->set_cost_matrix(matrix);
	bnb->execute();

	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	std::cout << search_time << std::endl;
	std::cout << search_time / CLOCKS_PER_SEC << std::endl;
	char h;
	std::cin >> &h;
    return 0;
}

