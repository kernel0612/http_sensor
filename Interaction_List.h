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
	 int delete_one_interaction(interaction* in);

	 int set_matched_client_isChunked(struct clientInfo* client,int chunked);
	 int malloc_matched_client_content(struct clientInfo* client,int len);
	 int set_matched_client_isComplete(struct clientInfo* client,int complete);
	 int set_matched_client_new_sip_info(struct clientInfo* client,char requestID[],char responseID[]
	 ,char contentType[],char resCode[],char date[]);
	 int set_matched_client_chunked_content(struct clientInfo* client,char chunked[],int len);
	 int set_matched_client_not_chunked_content(struct clientInfo* client,char content[],int len);
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