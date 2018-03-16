#pragma once
#include "Matrix.h"
#include "Tree.h"
#include "List.h"
#include <list>

class ExecuteBranchNBound
{
private:
	int min_path_cost_;
	Matrix* cost_matrix_;
	Tree* dec_tree_;
	std::list<Tree*> leaf_elems;
	int best_trip_;
	Tree* best_trip_leaf_;
	unsigned int overall_vertex_count_;

	int execute_with_additional_memory(Tree*, Tree**);
	Tree* get_next_tree_node(int*);

public:
	ExecuteBranchNBound();
	~ExecuteBranchNBound();
	int execute();
	void set_cost_matrix(Matrix*);
	void print_results();
};

