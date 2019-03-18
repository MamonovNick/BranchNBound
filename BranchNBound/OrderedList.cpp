#include "stdafx.h"
#include "OrderedList.h"

template <typename T>
OrderedList<T>::OrderedList(double T::* c_val)
{
	count_ = 0;
	comparison_value_ = c_val;
}

template <typename T>
OrderedList<T>::~OrderedList()
{
	clear();
}

template <typename T>
void OrderedList<T>::add(T* elem)
{
	if (!elem)
		return;
	//Current insert element value
	double insert_val = (*elem).*comparison_value_;

	//If list is empty simply add new element
	if (!order_list_)
	{
		Node<T>* tmp = new Node<T>();
		tmp->prev = nullptr;
		tmp->next = nullptr;
		tmp->elem = elem;
		order_list_ = tmp;
		count_++;
		return;
	}

	//Check for adding on first index
	//(We need to correct the main pointer)
	double cmp_val = (*(order_list_->elem)).*comparison_value_;
	if (insert_val <= cmp_val)
	{
		Node<T>* tmp = new Node<T>();
		tmp->prev = nullptr;
		tmp->next = order_list_;
		tmp->elem = elem;
		order_list_ = tmp;
		count_++;
		return;
	}

	Node<T>* tmp_ptr = order_list_;
	bool insert_done = false;
	while(tmp_ptr->next && !insert_done)
	{
		double next_elem_val = *((*(tmp_ptr->next)).elem).*comparison_value_;
		if(insert_val <= next_elem_val)
		{
			Node<T>* tmp = new Node<T>();
			tmp->prev = tmp_ptr;
			tmp->next = tmp_ptr->next;
			tmp->elem = elem;
			tmp_ptr->next = tmp;
			(tmp->next)->prev = tmp;
			count_++;
			insert_done = true;
		}
		tmp_ptr = tmp_ptr->next;
	}

	if(!insert_done)
	{
		Node<T>* tmp = new Node<T>();
		tmp->prev = tmp_ptr;
		tmp->next = nullptr;
		tmp->elem = elem;
		tmp_ptr->next = tmp;
		count_++;
	}
}

template <typename T>
T* OrderedList<T>::first()
{
	if (!order_list_)
		return nullptr;
	return order_list_->elem;
}

template <typename T>
T* OrderedList<T>::pull_first()
{
	if (!order_list_)
		return nullptr;
	Node<T>* tmp = order_list_;
	order_list_ = tmp->next;
	T* result = tmp->elem;
	tmp->elem = nullptr;
	delete tmp;
	count_--;

	return result;
}

template <typename T>
void OrderedList<T>::clear()
{
	while (order_list_)
	{
		Node<Tree>* tmp = order_list_;
		order_list_ = order_list_->next;
		delete tmp;
	}
}

template <typename T>
void OrderedList<T>::clear(double value)
{
	if (!order_list_)
		return;
	Node<T>* tmp_ptr = order_list_;
	while (tmp_ptr)
	{
		double elem_val = *(tmp_ptr->elem).*comparison_value_;
		if (elem_val > value)
			break;
		tmp_ptr = tmp_ptr->next;
	}
	while(tmp_ptr)
	{
		Node<Tree>* tmp = tmp_ptr;
		tmp_ptr = tmp_ptr->next;
		delete tmp;
	}
}
