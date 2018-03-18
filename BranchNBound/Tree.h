#pragma once
#include "Matrix.h"
#include "Arc.h"
#include "List.h"

//Goal tree structure
class Tree
{
private:


public:
	Matrix* cost_matrix;
	Arc* current_node_arc;
	List<Arc>* path_arcs;
	double lower_bound;
	double path_weight;
	bool is_final;

	Tree();
	~Tree();
};
