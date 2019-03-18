#pragma once
#include "Node.h"

template <typename T>
class List
{
	Node<T>* list;
	int count_;
public:
	List();
	~List();
	int count();
	T* add(T*);
	void simple_add(T*);
	List<T>* clone();
};
