#ifndef TIMEOUT_HANDLER_H
#define TIMEOUT_HANDLER_H
#include <memory>
#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/config-macros.h"
#include "commondef.h"
#include "interaction.h"
using namespace std;
class interaction;
class TimeOut_Handler:public ACE_Event_Handler
{
public:
	TimeOut_Handler(interaction* inter,unsigned int delay,ACE_Reactor* reactor);
	~TimeOut_Handler(void);
	//TimeOut_Handler(shared_ptr<interaction> sp_inter,unsigned int delay,ACE_Reactor* reactor);
	int set_timer();
	int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);
	int handle_close(ACE_HANDLE , ACE_Reactor_Mask );
	int cancel_timer();

private:
	
	int timeID;
	interaction* _interaction;
	ACE_Reactor* _reactor;
	ACE_Time_Value _tiv;
	ACE_Thread_Mutex _mutex;
//	shared_ptr<interaction> _sp_inter;
};

#endif