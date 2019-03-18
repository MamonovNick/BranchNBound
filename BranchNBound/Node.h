#pragma once

template <typename T>
class Node
{
public:
	T * elem;
	Node<T>* next;
	Node<T>* prev;
	Node()
	{
		
	}
	~Node()
	{
		if (elem)
			delete elem;
	}

	Node<T>* clone()
	{
		return nullptr;
	}
};
