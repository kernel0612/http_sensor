#ifndef INTERACTION_LIST_H
#define INTERACTION_LIST_H
#include "commondef.h"
#include <list>
#include <map>
#include <iostream>
#include "my_ace_guard.h"
#include "TimeOut_Handler.h"
using namespace std;
class Interaction_List
{
public:
	Interaction_List(ACE_Reactor* reactor);
public:
	~Interaction_List(void);
     int put(struct oneInteraction * p);
     int pop(struct oneInteraction** out);
     int clear();
	 unsigned int get_list_size();
	 struct oneInteraction* get_matched_interaction(char saddr[],char daddr[],unsigned short sPort);
	 TimeOut_Handler* get_matched_timeout_handler(struct oneInteraction* interaction);

private:
	unsigned int _count;
	list<struct oneInteraction*> _interractions;
	//list<TimeOut_Handler*> _timeout_handles;
	map<struct oneInteraction*,TimeOut_Handler*> _ITmap;
	ACE_Thread_Mutex  _mutex;
	ACE_Reactor* _reactor;
};


#endif