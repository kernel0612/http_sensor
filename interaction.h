#ifndef INTERACTION_H
#define INTERACTION_H
#include "commondef.h"
#include "TimeOut_Handler.h"
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
	_th=th;
}
inline  TimeOut_Handler* get_timeout_handler()
{
	return _th;
}

private:
	struct serverInfo* _server;
	struct clientInfo* _client;
	interaction_status _status;
	TimeOut_Handler* _th;
};

#endif 