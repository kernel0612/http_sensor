#ifndef TIMEOUT_HANDLER_H
#define TIMEOUT_HANDLER_H
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/config-macros.h"
#include "commondef.h"
class TimeOut_Handler:public ACE_Event_Handler
{
public:
	TimeOut_Handler(oneInteraction* interaction,unsigned int delay,ACE_Reactor* reactor);
	~TimeOut_Handler(void);

	int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);
	int cancel_timer();

private:
	ACE_Reactor_Mask _mask;
	int timeID;
	oneInteraction* _interaction;
	ACE_Reactor* _reactor;
};

#endif