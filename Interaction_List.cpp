#include "Interaction_List.h"

Interaction_List::Interaction_List(void)
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

struct clientInfo* Interaction_List::getMatchedClient(char saddr[],char daddr[],unsigned short sPort)
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
			return one->client;
		}
	}
	return 0;
}