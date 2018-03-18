#include "stdafx.h"
#include <climits>
#include "ExecuteBranchNBound.h"
#include <iostream>

ExecuteBranchNBound::ExecuteBranchNBound()
	: best_trip_(INT_MAX)
{
}


ExecuteBranchNBound::~ExecuteBranchNBound()
{
	delete cost_matrix_;
	std::list<Tree*>::iterator iter = leaf_elems.begin();
	while (iter != leaf_elems.end())
	{
		delete *iter;
		++iter;
	}
	leaf_elems.clear();
}

double ExecuteBranchNBound::execute()
{
	//Prepare tree and cost matrix
	tree_elems_count_ = 0;
	Tree* cur_ptr = new	Tree();
	Matrix* tmp_matrix = cost_matrix_->clone();
	double weight;
	tmp_matrix->matrix_reduction(&weight);
	cur_ptr->cost_matrix = tmp_matrix;
	cur_ptr->current_node_arc = nullptr;
	cur_ptr->lower_bound = weight;
	cur_ptr->path_arcs = new List<Arc>();

	//Start creating tree
	while(true)
	{
		double trip_weight = execute_with_additional_memory(cur_ptr);
		double possible_min_val;
		Tree* next_node = get_next_tree_node(&possible_min_val);

		if (!next_node->current_node_arc->is_include)
		{
			double h;
			next_node->cost_matrix->matrix_reduction(&h);
			//next_node->lower_bound += h;
		}

		/*if (trip_weight == -1)
		{
			//No tour found
			cur_ptr = next_node;
			continue;
		}*/

		if(trip_weight != -1 && trip_weight < best_trip_)
		{
			best_trip_ = trip_weight;
		}

		if(best_trip_ <= possible_min_val)
		{
			//Found min weight trip
			break;
		}

		cur_ptr = next_node;
	}

	return best_trip_;
}

void ExecuteBranchNBound::set_cost_matrix(Matrix* matrix)
{
	cost_matrix_ = matrix;
}

double ExecuteBranchNBound::execute_with_additional_memory(Tree* cur_node)
{
	if (cur_node->current_node_arc == nullptr)
		std::cout << "Start" << " -> ";
	else
		std::cout << cur_node->current_node_arc->toString() << " -> ";

	Matrix* cur_matrix = cur_node->cost_matrix;
	//cur_matrix->print_matrix();
	//Get branch arc
	double estimate;
	Arc* next_arc = cur_matrix->get_next_arc(&estimate);

	std::cout << next_arc->toString() << " : ";

	//Define set Y with branching arc
	Tree* tmp_right_tree = new Tree();
	tmp_right_tree->current_node_arc = next_arc->clone(true);
	tmp_right_tree->path_arcs = cur_node->path_arcs->clone();
	Arc* cor_arc = tmp_right_tree->path_arcs->add(next_arc);

	//Calc estimate
	Matrix* tmp_matrix = cur_matrix->create_cutted_copy(next_arc->first, next_arc->second);
	if (tmp_matrix->rows() > 2)
	{
		tmp_matrix->set_value(cor_arc->second, cor_arc->first, DBL_MAX);
	}
	double h_est;
	tmp_matrix->matrix_reduction(&h_est);
	tmp_right_tree->cost_matrix = tmp_matrix;
	tmp_right_tree->lower_bound = cur_node->lower_bound + h_est;

	std::cout << "(right)" << tmp_right_tree->lower_bound << " : ";

	//Define set Y without branching arc 
	Tree* tmp_left_tree = new Tree();
	tmp_left_tree->current_node_arc = next_arc->clone(false);
	tmp_left_tree->cost_matrix = cur_node->cost_matrix;

	//Exclude from consideration branching arc
	tmp_left_tree->cost_matrix->set_value(next_arc->first, next_arc->second, DBL_MAX);
	tmp_left_tree->path_arcs = cur_node->path_arcs;

	delete next_arc;
	delete cor_arc;

	//Reduce matrix
	double h = 0;
	//tmp_left_tree->cost_matrix->matrix_reduction(&h);

	//Calc estimate
	tmp_left_tree->lower_bound = cur_node->lower_bound + estimate + h;

	std::cout << "(left)" << tmp_left_tree->lower_bound << "\n";

	//Clearing
	//TODO Check matrix delete
	//Delete matrix in parent node
	cur_node->cost_matrix = nullptr;
	cur_node->path_arcs = nullptr;

	//Update list of leaf elems
	leaf_elems.remove(cur_node);
	leaf_elems.push_back(tmp_left_tree);
	delete cur_node;

	tree_elems_count_ += 2;

	leaf_elems.push_back(tmp_right_tree);

	if(tmp_matrix->rows() <= 2)
	{
		Arc** arcs = nullptr;
		double matr_min_elem = tmp_matrix->get_matrix_min_path(&arcs);
		if (matr_min_elem == -1)
			return -1;
		if(arcs != nullptr)
			std::cout << "Left arcs" << arcs[0]->toString() << " and " << arcs[1]->toString() << "\n";
		if(arcs != nullptr)
		{
			delete arcs[0];
			delete arcs[1];
			delete[] arcs;
		}
		tmp_right_tree->is_final = true;
		tmp_right_tree->path_weight = matr_min_elem + tmp_right_tree->lower_bound;
		return matr_min_elem + tmp_right_tree->lower_bound;
	}

	return -1;
}

Tree* ExecuteBranchNBound::get_next_tree_node(double* result_value)
{
	double min_value = DBL_MAX;
	Tree* result = nullptr;
	std::list<Tree*>::iterator iter = leaf_elems.begin();
	while(iter != leaf_elems.end())
	{
		if((*iter)->lower_bound < min_value)
		{
			min_value = (*iter)->lower_bound;
			result = *iter;
		}
		++iter;
	}

	*result_value = min_value;
	return result;
}
