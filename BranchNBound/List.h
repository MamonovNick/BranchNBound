#pragma once
#include "Arc.h"

template <typename T>
class Node
{
public:
	T * elem;
	Node<T>* next;
	Node<T>* prev;
	Node();
	~Node();
	//int first();
	//int second();
	//void set(int, int);
	Node<T>* clone();
};

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
