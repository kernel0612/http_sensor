#ifndef MY_RINGBUFFBASEDFIFO_H
#define MY_RINGBUFFBASEDFIFO_H
#include "my_fifo.h"

template<typename T>
class my_ringbuffbased_fifo :
	public my_fifo<T>
{
public:
	my_ringbuffbased_fifo(unsigned long size=1000);
public:
	~my_ringbuffbased_fifo(void);
	virtual int init();
	virtual int push_back(T content);
	virtual int pop_front(T& content);

private:
	T* base;

  
};
template<typename T>
my_ringbuffbased_fifo<T>::my_ringbuffbased_fifo(unsigned long size):my_fifo<T>(size),base(0)
{
}
template<typename T>
my_ringbuffbased_fifo<T>::~my_ringbuffbased_fifo(void)
{
	if (base)
	{
		delete [] base;
	}
}
template<typename T>
int my_ringbuffbased_fifo<T>::init()
{
	if (!base)
	{
		base=new T[this->maxSize];
		if (base)
		{
			this->full=0;
			return 0;
		}
	}
	return -1;
}
template<typename T>
int my_ringbuffbased_fifo<T>::push_back(T content)
{
	if (!base)
	{
		return -1;
	}
	ACE_Time_Value time_out_v=ACE_Time_Value(5)+ACE_OS::gettimeofday();
	int ret=0;
	{
		my_ace_guard guard(this->mutex);
		while (this->is_full()==0)
		{
			ret=this->condNotfull.wait(&time_out_v);
			if (ret==-1)
			{
				return 1;
			}
		}
		base[this->tail]=content;
		this->tail=(this->tail+1)%this->maxSize;
		if (this->head==this->tail)
		{
			this->full=1;
		}
		this->condNotempty.signal();

	}
	return 0;
}
template<typename T>
int my_ringbuffbased_fifo<T>::pop_front(T& content)
{
	if (!base)
	{
		return -1;
	}
	ACE_Time_Value time_out_v=ACE_Time_Value(5)+ACE_OS::gettimeofday();
	int ret=0;
	{
		my_ace_guard guard(this->mutex);
		while (this->is_empty()==0)
		{
			//printf("wait for not empty\n");
			ret=this->condNotempty.wait(&time_out_v);
			if (ret==-1)
			{
				return 1;
			}
		}
		content=base[this->head];
		this->head=(this->head+1)%this->maxSize;
		if (this->head==this->tail)
		{
			this->full=0;
		}
		this->condNotfull.signal();
	}
	return 0;
}

#endif