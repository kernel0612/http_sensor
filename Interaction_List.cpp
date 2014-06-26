#include "Interaction_List.h"

Interaction_List::Interaction_List():_notlessthan50(_mutex),_status(ENABLED)
{
}

Interaction_List::~Interaction_List(void)
{
	clear();
}

int Interaction_List::clear()
{
	my_ace_guard  guard(_mutex);
	list<interaction*>::iterator it=_inters.begin();
	while(it!=_inters.end())
	{
		if ((*it)->get_client_info())
		{
			if ((*it)->get_client_info()->content)
			{
				delete [](*it)->get_client_info()->content;
			}
			delete (*it)->get_client_info();
		}
		if ((*it)->get_server_info())
		{
			delete (*it)->get_server_info();
		}
		delete (*it);
		it=_inters.erase(it);
	}
	return -1;
}
int Interaction_List::delete_one_interaction(interaction* in)
{
	if (!in)
	{
		return -1;
	}
	if (in->get_client_info())
	{
		if (in->get_client_info()->content)
		{
			delete []in->get_client_info()->content;
		}
		delete in->get_client_info();
	}
	if (in->get_server_info())
	{
		delete in->get_server_info();
	}
	delete in;
	return 0;
}
unsigned int Interaction_List::get_list_size()
{
	my_ace_guard  guard(_mutex);
	return _inters.size();
}

int Interaction_List::put(interaction* in)
{
	if (!in)
	{
		return -1;
	}
	my_ace_guard  guard(_mutex);
	if (_status==DISABLED)
	{
		return 1;
	}
	_inters.push_back(in);
	if (_inters.size()>50)
	{
		_notlessthan50.signal();
	}
	return 0;
}
int Interaction_List::pop(interaction** out)
{
	if (!out)
	{
		return -1;
	}
	my_ace_guard  guard(_mutex);
	while(_inters.size()<50)
	{
		_notlessthan50.wait();
		if (_status==DISABLED)
		{
			return 1;
		}
	}
	if (_status==DISABLED)
	{
		return 1;
	}
	*out=(_inters.front());
	_inters.pop_front();
	return 0;
}
int Interaction_List::flush()
{
	return -1;
}
interaction* Interaction_List::get_matched_interaction_m(char saddr[],char daddr[],unsigned short sPort)
{
	if (!saddr||!daddr)
	{
		return 0;
	}
	list<interaction*>::const_reverse_iterator crit;
	interaction* one=0;
	my_ace_guard  guard(_mutex);

	for (crit=_inters.rbegin();crit!=_inters.rend();++crit)
	{
		one=(*crit);
		if (one)
		{
			if (one->get_server_info())
			{
				if (one->get_server_info()->srcPort==sPort&&strcmp(one->get_server_info()->src,saddr)==0
					&&strcmp(one->get_server_info()->des,daddr)==0)
				{
					if (one->get_interaction_status()==INTERACTION_NORMAL)
					{
						return one;
					}	
				}
			}
		}

	}
	return 0;
}
int Interaction_List::disabled()
{
	my_ace_guard  guard(_mutex);
	_status=DISABLED;
	_notlessthan50.broadcast();
	return 0;
}
int Interaction_List::enabled()
{
	my_ace_guard  guard(_mutex);
	_status=ENABLED;
	return 0;
}