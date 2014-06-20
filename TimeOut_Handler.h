#ifndef TIMEOUT_HANDLER_H
#define TIMEOUT_HANDLER_H
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
class TimeOut_Handler:public ACE_Event_Handler
{
public:
	TimeOut_Handler(unsigned int delay=5,ACE_Reactor*r=ACE_Reactor::instance());
	~TimeOut_Handler(void);

	int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);
	int cancel_timer();

private:
	ACE_Reactor_Mask _mask;
	int timeID;
};

#endif