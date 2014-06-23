#include "Interaction_List.h"

Interaction_List::Interaction_List(ACE_Reactor* reactor):_reactor(reactor)
{
}

Interaction_List::~Interaction_List(void)
{

}

int Interaction_List::put(struct oneInteraction * p)
{
	if (!p)
	{
		return -1;
	}
	my_ace_guard  guard(_mutex);
	_interractions.push_back(p);
	TimeOut_Handler *th=new TimeOut_Handler(p,10,_reactor);
	//_timeout_handles.push_back(th);
	_ITmap.insert(make_pair(p,th));
	++_count;
	return 0;
}
int Interaction_List::pop(struct oneInteraction** out)  //调用方负责释放内存
{
	if (!out)
	{
		return -1;
	}
	my_ace_guard  guard(_mutex);
	*out=(_interractions.front());
	_interractions.pop_front();
	--_count;
	return -1;
}
int Interaction_List::clear()
{
	my_ace_guard  guard(_mutex);
	list<struct oneInteraction*>::iterator it=_interractions.begin();
	while(it!=_interractions.end())
	{
		if ((*it)->client)
		{
			if ((*it)->client->content)
			{
				delete [](*it)->client->content;
			}
			delete (*it)->client;
		}
		if ((*it)->server)
		{
			delete (*it)->server;
		}
		delete (*it);
		it=_interractions.erase(it);
	}
	return -1;
}
unsigned int Interaction_List::get_list_size()
{
	my_ace_guard  guard(_mutex);
	return _count;
}

struct oneInteraction* Interaction_List::get_matched_interaction(char saddr[],char daddr[],unsigned short sPort)
{
	if (!saddr||!daddr)
	{
		return 0;
	}
	list<struct oneInteraction*>::const_reverse_iterator crit;
	struct oneInteraction* one=0;
	my_ace_guard  guard(_mutex);

	for (crit=_interractions.rbegin();crit!=_interractions.rend();++crit)
	{
		one=(*crit);
		if (one->server->srcPort==sPort&&strcmp(one->server->src,saddr)==0
			&&strcmp(one->server->des,daddr)==0)
		{
			if (one->status==INTERACTION_NORMAL)
			{
				return one;
			}	
		}
	}
	return 0;
}
TimeOut_Handler* Interaction_List::get_matched_timeout_handler(struct oneInteraction* interaction)
{
	my_ace_guard  guard(_mutex);
	TimeOut_Handler* th=0;
	map<struct oneInteraction*,TimeOut_Handler*>::const_iterator coit=_ITmap.find(interaction);
	if (coit==_ITmap.end())
	{
		return 0;
	}
	th=coit->second;
	_ITmap.erase(coit);
	return th;
}