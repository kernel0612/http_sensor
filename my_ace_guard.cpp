#include "my_ace_guard.h"


my_ace_guard::my_ace_guard(ACE_Thread_Mutex& mutex):_mutex(mutex)
{
	_mutex.acquire(0);
}
my_ace_guard::~my_ace_guard(void)
{
	_mutex.release();

}
