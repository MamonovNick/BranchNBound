#pragma once
#include "Node.h"
#include "Tree.h"

template <typename T>
class OrderedList
{
	Node<T>* order_list_;
	double T::*comparison_value_;
	int count_;
public:
	OrderedList(double T::*);
	~OrderedList();
	void add(T*);
	T* first();
	T* pull_first();
	void clear();
	void clear(double);
};

template class OrderedList<Tree>;
