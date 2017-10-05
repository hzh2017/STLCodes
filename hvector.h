// #ifndef _HVECTOR_H
// #define _HVECTOR_H

#pragma once

#include <iostream>
#include <assert.h>


namespace hzh
{

template<class T>
class hvector
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;

	hvector() : start(NULL), finish(NULL), end_of_storage(NULL) {}
	hvector(size_type n, const T& val = 0);
	~hvector();

	pointer begin() { return start; }
	pointer end() { return finish; }
	reference front() { return *start; }
	reference back() { return *finish; }

	void insert(pointer pos, const_reference val);
	void push_back(const_reference val);
	T erase(pointer pos);
	T pop_back();

	size_type size() { return finish - start + 1; }
	size_type capacity() { return end_of_storage - start + 1; }
	void resize(size_type new_size);
	bool empty() { return start >= finish; }
	bool full() { return finish >= end_of_storage; }
	void print();
	
	reference operator[] (size_type index) { return *(start + index); }

protected:
	pointer start;
	pointer finish;
	pointer end_of_storage;
};


template<class T>
hvector<T>::hvector(size_type n, const_reference val)
{
	assert(n > 0);

	start = new T[n];
	// start = (T*)malloc(n * sizeof(T));
	finish = start + n - 1;
	end_of_storage = finish;

	for(size_type i = 0; i < n; i++)
		start[i] = val;
}

template<class T>
hvector<T>::~hvector()
{
	if(!empty())
		delete[] start;
}

template<class T>
void hvector<T>::insert(T* pos, const_reference val)
{
	size_type index = pos - start;

	if(full())
		resize(size() * 2);

	if(index == size() - 1)
		start[size()] = val;
	else
	{
		for(size_type i = size() - 1; i >= index; i--)
		{
			start[i + 1] = start[i];
			if(i == 0)  // size_t 为无符号整数
				break;
		}
		start[index] = val;
	}
	finish++;
}

template<class T>
void hvector<T>::push_back(const T& val)
{
	insert(finish, val);
}

template<class T>
T hvector<T>::erase(pointer pos)
{
	assert(!empty());
	
	size_t index = pos - start;
	T ret = start[index];

	if(pos != finish)
	{	
		for(size_t i = index; i < size() - 1; i++)
			start[i] = start[i + 1];
	}
	finish--;

	return ret;
}

template<class T>
T hvector<T>::pop_back()
{
	assert(!empty());

	T ret = *finish--;
	return ret;
}

template<class T>
void hvector<T>::resize(size_t new_size)
{
	size_t old_size = size();
	pointer new_start = new T[new_size];
	for(size_t i = 0; i < old_size; i++)
		new_start[i] = start[i];
	start = new_start;
	finish = start + old_size - 1;
	end_of_storage = start + new_size - 1;
}

template<class T>
void hvector<T>::print()
{
	assert(!empty());

	for(size_t i = 0; i < size(); i++)
		std::cout << start[i] << std::ends;
	std::cout << std::endl;
}

void test_hvector()
{
	hvector<int> a(5);
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	a.push_back(5);
	a.insert(a.begin(), 8);
	a.insert(a.begin(), 7);
	a.insert(a.begin(), 6);
	a.print();
	std::cout << "erase = " << a.erase(a.end()) << std::endl;
	a.print();
	std::cout << "erase = " << a.erase(a.begin()) << std::endl;
	a.print();
	std::cout << a[0] << std::endl;
}

}

// #endif