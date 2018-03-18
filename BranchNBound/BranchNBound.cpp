// BranchNBound.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matrix.h"
#include <climits>
#include <cstdlib>
#include "ExecuteBranchNBound.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <math.h>


int main()
{
	std::string filepath = "F:\\BnBOut";

	int iter_count = 20;
	double delta_start = 0;
	double delta_max = 1.1;
	double delta_step = 0.05;
	int times_count = static_cast<int>((delta_max - delta_start) / delta_step);

	for(int cities = 5; cities <= 35; cities += 5)
	{
		//Цикл по количеству городов
		//Сохраняем в свой файл
		std::string filename =  "Results_" + std::to_string(cities);
		filename = filename + ".txt";

		std::ofstream out;
		out.open(filepath + "\\" + filename);

		double* average_complexity = new double[times_count];
		for (int l = 0; l < times_count; l++)
			average_complexity[l] = 0;

		for (int k = 0; k < iter_count; k++)
		{

			Matrix* matrix = new Matrix(cities);
			matrix->create_symmetric();

			int index = 0;
			double delta = delta_start;
			while (delta <= delta_max)
			{
				std::cout << delta << std::endl;
				unsigned int start_time = clock(); // начальное время
				Matrix* matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 1);
				matrix_local->print_matrix();
				ExecuteBranchNBound* bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				unsigned int end_time = clock(); // конечное время
				unsigned int search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity[index] = average_complexity[index++] + bnb->tree_complexity();
				
				delta += delta_step;
				delete bnb;
			}

			delete matrix;
		}

		int index = 0;
		double delta = delta_start;
		while (delta <= delta_max)
		{
			out << delta << " " << average_complexity[index++] / iter_count << "\n";
			delta += delta_step;
		}

		delete[] average_complexity;

		out.close();
	}

	//unsigned int start_time = clock(); // начальное время

	//Matrix* matrix = new Matrix(5);
	//
	////
	//matrix->create_asyn_matrix(0.1, 1, 0);
	////

	//matrix->print_matrix();

	//ExecuteBranchNBound* bnb = new ExecuteBranchNBound();
	//bnb->set_cost_matrix(matrix);
	//bnb->execute();
	//std::cout << "Min path = " << bnb->best_trip() << std::endl;

	//unsigned int end_time = clock(); // конечное время
	//unsigned int search_time = end_time - start_time; // искомое время
	//std::cout << "SearchTime = " << search_time << std::endl;
	//std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;
	//char h;
	//std::cin >> &h;
    return 0;
}

