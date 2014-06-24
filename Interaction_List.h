#ifndef INTERACTION_LIST_H
#define INTERACTION_LIST_H
#include "commondef.h"
#include <list>
#include <map>
#include <iostream>
#include "my_ace_guard.h"
#include "TimeOut_Handler.h"
#include "interaction.h"
using namespace std;

class Interaction_List
{
public:
	Interaction_List();
public:
	~Interaction_List(void);
     int clear();
	 unsigned int get_list_size();
	 int put(interaction* in);
	 int pop(interaction** out);
	 int flush();
	 interaction* get_matched_interaction_m(char saddr[],char daddr[],unsigned short sPort);
	 int disabled();
	 int enabled();

private:
	unsigned int _count;
	ACE_Thread_Mutex  _mutex;
	list<interaction*> _inters;
	ACE_Condition<ACE_Thread_Mutex> _notlessthan50;
	typedef enum
	{
		ENABLED,
		DISABLED
	}STATUS;
	STATUS _status;

};


#endif