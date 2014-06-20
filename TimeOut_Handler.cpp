#include "TimeOut_Handler.h"


TimeOut_Handler::TimeOut_Handler(unsigned int delay,ACE_Reactor*r):ACE_Event_Handler(r)
{
	ACE_Time_Value tiv(delay);
	timeID=reactor()->schedule_timer(this,0,tiv);
}
TimeOut_Handler::~TimeOut_Handler(void)
{
}
int TimeOut_Handler::handle_timeout(const ACE_Time_Value &current_time, const void *act )
{
	ACE_DEBUG((LM_DEBUG,"time out %d\n",timeID));
	return -1;
}
int TimeOut_Handler::cancel_timer()
{
	return reactor()->cancel_timer(timeID);
}