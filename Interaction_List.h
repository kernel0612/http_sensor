#ifndef INTERACTION_LIST_H
#define INTERACTION_LIST_H
#include "commondef.h"
#include <list>
#include <iostream>
#include "my_ace_guard.h"
using namespace std;
class Interaction_List
{
public:
	Interaction_List(void);
public:
	~Interaction_List(void);
     int put(struct oneInteraction * p);
     int pop(struct oneInteraction** out);
     int clear();
	 unsigned int get_list_size();
	 struct clientInfo* getMatchedClient(char saddr[],char daddr[],unsigned short sPort);


private:
	unsigned int _count;
	list<struct oneInteraction*> _interractions;
	ACE_Thread_Mutex  _mutex;
};


#endif