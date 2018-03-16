#include "stdafx.h"
#include "Tree.h"


Tree::Tree()
{

}

Tree::Tree(Tree* parent_node)
{

}


Tree::~Tree()
{
	delete current_node_arc;
}
