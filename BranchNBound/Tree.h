#pragma once
#include "Matrix.h"
#include "Arc.h"
#include "List.h"

//Goal tree structure
class Tree
{
private:


public:
	//Tree* parent_node_;
	//Tree* left_tree_;
	//Tree* right_tree_;
	Matrix* cost_matrix;
	Arc* current_node_arc;
	List<Arc>* path_arcs;
	int lower_bound;
	int path_weight;
	bool is_final;

	Tree();
	Tree(Tree*);
	~Tree();
};
