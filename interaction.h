#ifndef INTERACTION_H
#define INTERACTION_H
#include <memory>
#include "commondef.h"
#include "TimeOut_Handler.h"
#include "my_ace_guard.h"
using namespace std;
class TimeOut_Handler;
class interaction
{
public:
	interaction(void);
	~interaction(void);
	inline	void set_server_info(struct serverInfo* server)
	{
		_server=server;
	}
	inline	struct serverInfo* get_server_info()
	{
		return _server;
	}
	inline	void set_client_info(struct clientInfo* client)
	{
		_client=client;
	}
	inline	struct clientInfo* get_client_info()
	{
		return _client;
	}
	inline	void set_interactin_status(interaction_status status)
	{
		_status=status;
	}
	inline	interaction_status get_interaction_status()
	{
		return _status;
	}
	inline  void set_timeout_handler(TimeOut_Handler* th)
	{
		//my_ace_guard guard(_mutex);
		_th=th;
	}
	//inline void set_timeout_handler(weak_ptr<TimeOut_Handler> wk_th)
	//{
	//	_wk_th=wk_th;
	//}
	inline  TimeOut_Handler* get_timeout_handler()
	{
		//my_ace_guard guard(_mutex);
		return _th;
	}
	//inline  weak_ptr<TimeOut_Handler>& get_timeout_handler_wk()
	//{
	//	return _wk_th;
	//}
	inline ACE_Thread_Mutex& get_mutex()
	{
		return _mutex;
	}
	
private:
	struct serverInfo* _server;
	struct clientInfo* _client;
	interaction_status _status;
	TimeOut_Handler* _th;
	ACE_Thread_Mutex  _mutex;
	//weak_ptr<TimeOut_Handler> _wk_th;
	
};

#endif 