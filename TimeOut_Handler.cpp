#include "TimeOut_Handler.h"


TimeOut_Handler::TimeOut_Handler(interaction* inter,unsigned int delay,ACE_Reactor* reactor):
ACE_Event_Handler(reactor),_interaction(inter),_reactor(reactor),_tiv(delay)
{

}
//TimeOut_Handler::TimeOut_Handler(shared_ptr<interaction> sp_inter,unsigned int delay,ACE_Reactor* reactor):
//ACE_Event_Handler(reactor),_reactor(reactor),_tiv(delay),_sp_inter(sp_inter)
//{
//
//}
TimeOut_Handler::~TimeOut_Handler(void)
{
	ACE_DEBUG((LM_DEBUG,"~TimeOut_Handler %d\n",timeID));
}
int TimeOut_Handler::handle_timeout(const ACE_Time_Value &current_time, const void *act )
{
	ACE_DEBUG((LM_DEBUG,"time out %d\n",timeID));
	if (_interaction)
	{
    	_interaction->set_interactin_status(INTERACTION_TIMEOUT);
	}
	//if (_sp_inter)
	//{
	//	_sp_inter->set_interactin_status(INTERACTION_TIMEOUT);
	//}
	return -1;
}
int TimeOut_Handler::set_timer()
{
	return timeID=_reactor->schedule_timer(this,0,_tiv);
}
int TimeOut_Handler::cancel_timer()
{
	int ret=0;
	ret=_reactor->cancel_timer(timeID);
	delete this;
	return ret;
}
int TimeOut_Handler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
	delete this;
	return 0;
}