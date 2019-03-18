#include "stdafx.h"
#include "List.h"
#include "Arc.h"

template<typename T>
List<T>::List()
{

}

//template <typename T>
List<Arc>::~List()
{
	Node<Arc>* ptr = list;
	while(ptr)
	{
		Node<Arc>* tmp_ptr = ptr->next;
		delete ptr;
		ptr = tmp_ptr;
	}
}

template <typename T>
int List<T>::count()
{
	return count_;
}

Arc* List<Arc>::add(Arc* arc)
{
	Arc* result = new Arc();
	Node<Arc>* tmp_ptr = list;
	Node<Arc>* corrected_arc = nullptr;
	int fst = arc->first;
	int snd = arc->second;
	int corrected = 0;

	while(tmp_ptr)
	{
		//TODO Delete this if, because if everything implement right it's no need to check this
		if(fst == tmp_ptr->elem->first)
		{
			//Error
			//TODO Impement error processing
			return nullptr;
		}
		if (fst == tmp_ptr->elem->second)
		{
			tmp_ptr->elem->second = snd;
			result->first = tmp_ptr->elem->first;
			result->second = tmp_ptr->elem->second;
			corrected = 2;
			corrected_arc = tmp_ptr;
			break;
		}
		if(!corrected && snd == tmp_ptr->elem->first)
		{
			tmp_ptr->elem->first = fst;
			result->first = tmp_ptr->elem->first;
			result->second = tmp_ptr->elem->second;
			corrected = 1;
			corrected_arc = tmp_ptr;
			break;
		}
		tmp_ptr = tmp_ptr->next;
	}

	if (corrected)
	{
		//Check list for concatenation
		tmp_ptr = list;
		while(tmp_ptr)
		{
			if (tmp_ptr->elem->first == result->first && tmp_ptr->elem->second == result->second)
			{
				//skip
				tmp_ptr = tmp_ptr->next;
				continue;
			}
			if(result->first == tmp_ptr->elem->second)
			{
				corrected_arc->elem->second = result->second;
				if (tmp_ptr->prev != nullptr)
					tmp_ptr->prev->next = tmp_ptr->next;
				if (tmp_ptr->next != nullptr)
					tmp_ptr->next->prev = tmp_ptr->prev;
				delete tmp_ptr;
				break;
			}
			if (result->second == tmp_ptr->elem->first)
			{
				corrected_arc->elem->first = result->first;
				if (tmp_ptr->prev != nullptr)
					tmp_ptr->prev->next = tmp_ptr->next;
				if (tmp_ptr->next != nullptr)
					tmp_ptr->next->prev = tmp_ptr->prev;
				delete tmp_ptr;
				break;
			}
			tmp_ptr = tmp_ptr->next;
		}
	}
	else
	{
		count_++;
		simple_add(arc);
		result->first = arc->first;
		result->second = arc->second;
	}

	return result;
}

template <>
void List<Arc>::simple_add(Arc* arc)
{
	Node<Arc>* tmp_ptr = list;
	Node<Arc>* new_elem = new Node<Arc>();
	new_elem->elem = new Arc();
	new_elem->elem->first = arc->first;
	new_elem->elem->second = arc->second;
	new_elem->next = nullptr;

	if (tmp_ptr == nullptr)
	{
		list = new_elem;
		return;
	}
	while (tmp_ptr->next)
		tmp_ptr = tmp_ptr->next;
	tmp_ptr->next = new_elem;
	new_elem->prev = tmp_ptr;
}

List<Arc>* List<Arc>::clone()
{
	List<Arc>* result = new List<Arc>();
	
	if (list == nullptr)
	{
		result->count_ = 0;
		result->list = nullptr;
		return result;
	}

	Node<Arc>* tmp_ptr = list;
	Node<Arc>* new_ptr, * tmp_new_ptr;
	new_ptr = tmp_new_ptr = new Node<Arc>();
	tmp_new_ptr->elem = new Arc();
	tmp_new_ptr->elem->first = tmp_ptr->elem->first;
	tmp_new_ptr->elem->second = tmp_ptr->elem->second;
	tmp_ptr = tmp_ptr->next;
	result->count_++;
	while(tmp_ptr)
	{
		tmp_new_ptr->next = new Node<Arc>();
		tmp_new_ptr->next->prev = tmp_new_ptr;
		tmp_new_ptr = tmp_new_ptr->next;
		tmp_new_ptr->elem = new Arc();
		tmp_new_ptr->elem->first = tmp_ptr->elem->first;
		tmp_new_ptr->elem->second = tmp_ptr->elem->second;
		tmp_ptr = tmp_ptr->next;
		result->count_++;
	}
	result->list = new_ptr;

	return result;
}
