#include "TimeOut_Handler.h"


TimeOut_Handler::TimeOut_Handler(oneInteraction* interaction,unsigned int delay,ACE_Reactor* reactor):
ACE_Event_Handler(reactor),_interaction(interaction),_reactor(reactor)
{
	ACE_Time_Value tiv(delay);
	timeID=_reactor->schedule_timer(this,0,tiv);
}
TimeOut_Handler::~TimeOut_Handler(void)
{

}
int TimeOut_Handler::handle_timeout(const ACE_Time_Value &current_time, const void *act )
{
	ACE_DEBUG((LM_DEBUG,"time out %d\n",timeID));
	if (_interaction)
	{
    	_interaction->status=INTERACTION_TIMEOUT;
	}
	return -1;
}
int TimeOut_Handler::cancel_timer()
{
	return _reactor->cancel_timer(timeID);
}