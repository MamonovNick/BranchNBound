#pragma once
#include "Matrix.h"
#include "Tree.h"
#include "OrderedList.h"

class ExecuteBranchNBound
{
private:
	double min_path_cost_;
	Matrix* cost_matrix_;
	OrderedList<Tree>* leaf_elems;
	double best_trip_;
	int tree_elems_count_;

	double execute_with_additional_memory(Tree*);
	Tree* get_next_tree_node(double*);

public:
	ExecuteBranchNBound();
	~ExecuteBranchNBound();
	double execute();
	void set_cost_matrix(Matrix*);
	double best_trip() { return best_trip_;  };
	int tree_complexity() { return tree_elems_count_; }
};

