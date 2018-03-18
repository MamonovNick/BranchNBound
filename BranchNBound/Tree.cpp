#include "stdafx.h"
#include "Tree.h"


Tree::Tree()
{

}

Tree::~Tree()
{
	delete cost_matrix;
	delete current_node_arc;
	delete path_arcs;
}
