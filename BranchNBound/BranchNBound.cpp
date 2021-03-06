// BranchNBound.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matrix.h"
#include <cstdlib>
#include "ExecuteBranchNBound.h"
#include <ctime>
#include <iostream>
#include <fstream>

int main()
{
	std::string filepath = "F:\\BnBOut\\Results1";
	//std::string filepath = "D:\\BnBOut\\FullCollection3";

	int iter_count = 40; //Number of iteration for each cities count
	double delta_start = 0; //Delta start
	double delta_max = 0.0501;//0.05; //Delta stop
	double delta_step = 0.005;//0.005; //Delta step
	int times_count = static_cast<int>((delta_max - delta_start) / delta_step) + 1; //Define arrays dimensions for storing average

	//Main loop on cities count
	for (int cities = 5; cities <= 20; cities += 5)
	{
		std::cout << "||||| " << cities << std::endl;

		//Save results to own file
		std::string filename = "Results_" + std::to_string(cities);
		filename = filename + ".txt";

		std::ofstream out1, out2, out3, out4, out5, out6;
		out1.open(filepath + "\\" + "1var" + filename);
		out2.open(filepath + "\\" + "2var" + filename);
		out3.open(filepath + "\\" + "3var" + filename);
		out4.open(filepath + "\\" + "4var" + filename);
		out5.open(filepath + "\\" + "5var" + filename);
		out6.open(filepath + "\\" + "6var" + filename);

		double* average_complexity = new double[times_count]; //For delta 1 type
		double* average_complexity2 = new double[times_count]; //For delta 2 type
		double* average_complexity3 = new double[times_count]; //For delta 3 type
		double* average_complexity4 = new double[times_count]; //For delta 4 type
		double* average_complexity5 = new double[times_count]; //For delta 5 type
		double* average_complexity6 = new double[times_count]; //For delta 6 type
		//Initialize arrays
		for (int l = 0; l < times_count; l++)
		{
			average_complexity[l] = 0;
			average_complexity2[l] = 0;
			average_complexity3[l] = 0;
			average_complexity4[l] = 0;
			average_complexity5[l] = 0;
			average_complexity6[l] = 0;
		}

		//Loop on iterations
		for (int k = 0; k < iter_count; k++)
		{
			Matrix* matrix = new Matrix(cities);
			matrix->create_symmetric();

			double delta = delta_start; //Current delta
			for(int index = 0; index < times_count; index++, delta += delta_step)
			{
				std::cout << delta << std::endl;

				unsigned int start_time = clock(); //Start time
				Matrix* matrix_local = matrix->clone(); //Create local copy of matrix
				matrix_local->add_delta(delta, 1); // Add asymmetry to matrix
				ExecuteBranchNBound* bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				unsigned int end_time = clock(); //End time
				unsigned int search_time = end_time - start_time; //Execution time
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				//Count complexity
				average_complexity[index] = average_complexity[index] + bnb->tree_complexity();

				delete bnb;

				start_time = clock(); // начальное время
				matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 2);
				bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity2[index] = average_complexity2[index] + bnb->tree_complexity();
				
				delete bnb;

				start_time = clock(); // начальное время
				matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 3);
				bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity3[index] = average_complexity3[index] + bnb->tree_complexity();

				delete bnb;

				start_time = clock(); // начальное время
				matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 4);
				bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity4[index] = average_complexity4[index] + bnb->tree_complexity();

				delete bnb;

				start_time = clock(); // начальное время
				matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 5);
				bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity5[index] = average_complexity5[index] + bnb->tree_complexity();

				delete bnb;

				start_time = clock(); // начальное время
				matrix_local = matrix->clone();
				matrix_local->add_delta(delta, 6);
				bnb = new ExecuteBranchNBound();
				bnb->set_cost_matrix(matrix_local);
				bnb->execute();

				end_time = clock(); // конечное время
				search_time = end_time - start_time; // искомое время
				std::cout << "SearchTime = " << search_time << std::endl;
				std::cout << "SearchTime = " << search_time / CLOCKS_PER_SEC << std::endl;

				average_complexity6[index] = average_complexity6[index] + bnb->tree_complexity();

				delete bnb;
			}

			delete matrix;
		}

		double delta = delta_start;
		for (int index = 0; index < times_count; index++, delta += delta_step)
		{
			out1 << delta << " " << average_complexity[index] / iter_count << "\n";
			out2 << delta << " " << average_complexity2[index] / iter_count << "\n";
			out3 << delta << " " << average_complexity3[index] / iter_count << "\n";
			out4 << delta << " " << average_complexity4[index] / iter_count << "\n";
			out5 << delta << " " << average_complexity5[index] / iter_count << "\n";
			out6 << delta << " " << average_complexity6[index] / iter_count << "\n";
		}

		delete[] average_complexity;
		delete[] average_complexity2;
		delete[] average_complexity3;
		delete[] average_complexity4;
		delete[] average_complexity5;
		delete[] average_complexity6;

		out1.close();
		out2.close();
		out3.close();
		out4.close();
		out5.close();
		out6.close();
	}

	return 0;
}
