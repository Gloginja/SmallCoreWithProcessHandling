/*
 * list.h
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_

#include "datadef.h"
#include <iostream.h>
template <class T>
class List
{
public:
	struct Element
	{
		T data;
		Element* prev,*next;
		Element(T _data)
		{
			data = _data;
			prev = nullptr;
			next = nullptr;
		}
	};
private:
	unsigned int n;
	Element* first, *last, *current;
public:

	List()
	{
		this->first=nullptr;
		this->last=nullptr;
		this->current=nullptr;
		this->n=0;
	}

	unsigned int getCount()
	{
		return n;
	}

	Element* getCurrent()
	{
		return this->current;
	}

	Element* getFirst()
	{
		return this->first;
	}

	Element* getLast()
	{
		return this->last;
	}

	unsigned int getSize()
	{
		return this->n;
	}

	void pushBack(T data)
	{
		Element* e = new Element(data);
		if(n==0)
		{
			this->first = e;
			this->last = e;
			this->current = e;
			n++;

		}
		else
		{
			this->last->next = e;
			e->prev = this->last;
			this->last = e;
			n++;
			//cout << "listCount " << n << endl;
		}
	}

	void pushFront(T data)
	{
		Element* e = new Element(data);
		if(n==0)
		{

			this->first = e;
			this->last = e;
			this->current = e;
			n++;
		}
		else
		{
			e->next = this->first;
			this->first->prev = e;
			this->first = e;
			n++;
		}
	}

	T popBack()
	{
		if(n==1)
		{
			T data = this->first->data;
			Element* toErase = this->first;
			this->last = nullptr;
			this->first= nullptr;
			this->current= nullptr;
			delete toErase;
			n--;
			return data;
		}else
		if(n!=0)
		{
			T data = this->last->data;
			Element* toErase = this->last;
			this->last = this->last->prev;
			this->last->next = nullptr;
			delete toErase;
			n--;
			return data;
		}
		else return nullptr;
	}

	T popFront()
		{

		if(n==1)
		{
			T data = this->last->data;
			Element* toErase = this->last;
			this->last = nullptr;
			this->first= nullptr;
			this->current= nullptr;
			delete toErase;
			n--;
			return data;
		}else
			if(n!=0)
			{
				T data = this->first->data;
				Element* toErase = this->first;
				this->first = this->first->next;
				this->first->prev = nullptr;
				delete toErase;
				n--;
				//resetCurrent();
				return data;
			}
			else return nullptr;
		}

	void nextCurrent()
	{
			this->current = this->current->next;
	}

	void prevCurrent()
		{
			if(this->current->prev!=nullptr)
			{
				this->current = this->current->prev;
			}
		}

	void resetCurrent()
	{
		this->current = this->first;
	}

	boolean isLast()
	{
		if(this->current == nullptr)
		return true;
		else return false;
	}

	void erase()
	{
		if(this->current!=nullptr)
		{
			Element* toErase = this->current;

			if(this->current==this->last)
			{
				this->last=this->current->prev;
			}

			if(this->current==this->first)
			{
				this->first=this->current->next;
			}

			if(this->current->prev!=nullptr)
				this->current->prev->next = this->current->next;
			if(this->current->next!=nullptr)
				this->current->next->prev = this->current->prev;

			this->current = this->current->next;

			delete toErase;
			n--;
		}
	}

	void positionCurrent(T t)
	{
		Element* tmp = first;
		while(!tmp)
		{
			if(tmp->data==t) break;
			else tmp=tmp->next;
		}
		//resetCurrent();
	}

	void clear()
	{
		while(this->first!=nullptr)
		{
			Element* toErase = this->first;
			this->first = this->first->next;
			delete toErase;
			n--;
		}
		this->first=nullptr;
		this->last=nullptr;
		this->current=nullptr;
	}

	boolean isEmpty()
	{
		if(n==0) return true;
		else
			return false;
	}

	//void copy();

	~List()
	{
		clear();
	}
};



#endif /* H_LIST_H_ */
