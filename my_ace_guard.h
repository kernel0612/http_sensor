#ifndef MY_ACE_GUARD_H
#define MY_ACE_GUARD_H
#include "ace/Synch.h"

class my_ace_guard
{
public:
	my_ace_guard(ACE_Thread_Mutex& mutex);
	~my_ace_guard(void);

private:
	ACE_Thread_Mutex& _mutex;
};

#endif