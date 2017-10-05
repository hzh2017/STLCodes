// #ifndef _HLIST_H
// #define _HLIST_H

#pragma once

#include <iostream>


namespace hzh
{

template<class T>
struct _hlist_node
{
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;

	_hlist_node() : prev(NULL), next(NULL), data(0) {}
	_hlist_node(const T& val) : prev(NULL), next(NULL), data(val) {}
};


template<class T>
struct _hlist_iterator
{
	typedef _hlist_iterator<T> iterator;
	typedef _hlist_iterator<T>& iterator_reference;
	typedef const _hlist_iterator<T>& const_iterator_reference;
	typedef _hlist_iterator<T>* iterator_pointer;

	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef _hlist_node<T> hlist_node;
	typedef hlist_node* link_type;

	link_type node;

	//constructor
	_hlist_iterator() : node(NULL) {}
	_hlist_iterator(link_type x) : node(x) {}
	_hlist_iterator(const_iterator_reference x) : node(x.node) {}

	bool operator==(const_iterator_reference x) const { return node == x.node; }
	bool operator!=(const_iterator_reference x) const { return node != x.node; }
	reference operator*() const { return node->data; }

	iterator_reference operator++()
	{
		node = (link_type)(node->next);
		return *this;
	}

	iterator operator++(int)
	{
		iterator tmp = *this;
		++*this;
		return tmp;
	}

	iterator_reference operator--()
	{
		node = (link_type)(node->prev);
		return *this;
	}

	iterator operator--(int)
	{
		iterator tmp = *this;
		--*this;
		return tmp;
	}
};


template<class T>
class hlist
{
public:
	typedef _hlist_node<T> hlist_node;
	typedef hlist_node* link_type;
	typedef _hlist_iterator<T> iterator;

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;

	hlist();

	iterator begin() { return link_type(node->next); }
	iterator end() { return node; }

	bool empty() { return node->next == node; }
	size_type size() const;
	reference front() { return begin()->data; }
	reference back() { return (--end())->data; }

	void push_back(const_reference val);
	iterator find(iterator itr1, iterator itr2, const_reference val);
	iterator erase(iterator itr);
	void print();

protected:
	link_type node;
};


template<class T>
hlist<T>::hlist()
{
	node = new _hlist_node<T>();
	node->prev = node;
	node->next = node;
}

template<class T>
size_t hlist<T>::size() const
{
	size_t result = 0;
	_hlist_iterator<T> itr = begin();
	while(itr != end())
	{
		itr++;
		result++;
	}
	return result;
}

template<class T>
void hlist<T>::push_back(const T& val)
{
	_hlist_node<T>* new_node = new _hlist_node<T>;
	_hlist_node<T>* end_prev_node = (_hlist_node<T>*)end().node->prev;

	end_prev_node->next = new_node;
	end().node->prev = new_node;

	new_node->next = end().node;
	new_node->prev = end_prev_node;
	new_node->data = val;
}

template<class T>
_hlist_iterator<T> hlist<T>::find(_hlist_iterator<T> itr1, _hlist_iterator<T> itr2, const T& val)
{
	for(_hlist_iterator<T> itr = itr1; itr != itr2; itr++)
	{
		if(*itr == val)
			return itr;
	}
	return 0;
}

template<class T>
_hlist_iterator<T> hlist<T>::erase(_hlist_iterator<T> itr)
{
	if(empty())
		return 0;
	
	_hlist_node<T>* itr_prev_node = (_hlist_node<T>*)itr.node->prev;
	_hlist_node<T>* itr_next_node = (_hlist_node<T>*)itr.node->next;
	itr_prev_node->next = itr_next_node;
	itr_next_node->prev = itr_prev_node;

	delete itr.node;

	return itr_prev_node;
}

template<class T>
void hlist<T>::print()
{
	for(_hlist_iterator<T> itr = begin(); itr != end(); itr++)
		std::cout << *itr << std::ends;
	std::cout << std::endl;
}

void test_hlist()
{
	hlist<int> hl;
	hl.push_back(0);
	hl.push_back(1);
	hl.push_back(2);
	hl.push_back(3);
	hl.print();
	_hlist_iterator<int> itr = hl.find(hl.begin(), hl.end(), 3);
	if(itr != 0)
		std::cout << *(hl.erase(itr)) << std::endl;
}


}

// #endif


/* const类成员函数小结
1.若将成员成员函数声明为const，则该函数不允许修改类的数据成员
2.const成员函数可以访问非const对象的非const数据成员、const数据成员，也可以访问const对象内的所有数据成员；
3.非const成员函数可以访问非const对象的非const数据成员、const数据成员，但不可以访问const对象的任意数据成员；
*/