#include "Interaction_List.h"

Interaction_List::Interaction_List():_notlessthan100(_mutex),_status(ENABLED)
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
	my_ace_guard  guard(_mutex);
	if (!in)
	{
		return -1;
	}
	if (in->get_client_info())
	{
		if (in->get_client_info()->content)
		{
			delete []in->get_client_info()->content;
			in->get_client_info()->content=0;
		}
		delete in->get_client_info();
		in->set_client_info(0);
	}
	if (in->get_server_info())
	{
		delete in->get_server_info();
		in->set_server_info(0);
	}
	if (in->get_timeout_handler())
	{
		in->get_timeout_handler()->cancel_timer();
		delete in->get_timeout_handler();
		in->set_timeout_handler(0);
	}
	delete in;
	in=0;
	return 0;
}
unsigned int Interaction_List::get_list_size()
{
	my_ace_guard  guard(_mutex);
	return _inters.size();
}

int Interaction_List::put(interaction* in)
{	
	my_ace_guard  guard(_mutex);
	if (!in)
	{
		return -1;
	}
	if (_status==DISABLED)
	{
		return 1;
	}
	_inters.push_back(in);
	if (_inters.size()>100)
	{
		_notlessthan100.signal();
	}
	return 0;
}
int Interaction_List::pop(interaction** out)
{
	my_ace_guard  guard(_mutex);
	if (!out)
	{
		return -1;
	}
	while(_inters.size()<100)
	{
		_notlessthan100.wait();
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
	_notlessthan100.broadcast();
	return 0;
}
int Interaction_List::enabled()
{
	my_ace_guard  guard(_mutex);
	_status=ENABLED;
	return 0;
}

int Interaction_List::set_matched_client_isChunked(struct clientInfo* client,int chunked)
{
	my_ace_guard  guard(_mutex);
	if (!client)
	{
		return -1;
	}
	client->isChunked=chunked;
	return 0;
}
int Interaction_List::malloc_matched_client_content(struct clientInfo* client,int len)
{
	my_ace_guard  guard(_mutex);
	if (!client||len<=0)
	{
		return -1;
	}
	if (client->complete)
	{
		return 0;
	}
	if (!client->content)
	{
		try
		{
			client->content=new char[len+1];
			memset(client->content,0,len);
			client->contentSize=len;
		}
		catch (...)
		{
			cout <<"out of memory"<<endl;
			::exit(1);
		}

	}
	return 0;

}
int Interaction_List::set_matched_client_isComplete(struct clientInfo* client,int complete)
{
	my_ace_guard  guard(_mutex);
	if (!client)
	{
		return -1;
	}
	client->complete=complete;
	return 0;
}
int Interaction_List::set_matched_client_new_sip_info(struct clientInfo* client,char requestID[],char responseID[]
,char contentType[],char resCode[],char date[])
{
	my_ace_guard  guard(_mutex);
	if (!client||!requestID||!responseID||!contentType||!resCode||!date)
	{
		return -1;
	}
	memcpy(client->responseID,responseID,RESPONSEID_BUF_SIZE);
	memcpy(client->requestID,requestID,REQUESTID_BUF_SIZE);
	memcpy(client->contentType,contentType,CONTENTTYPE_BUF_SIZE);
	memcpy(client->resCode,resCode,RESPONSECODE_BUF_SIZE);
	memcpy(client->date,date,DATE_BUF_SIZE);
	return 0;
}
int Interaction_List::set_matched_client_chunked_content(struct clientInfo* client,char chunked[],int len)
{
	char* tmp_client_content=0;
	int tmp_client_content_len=0;
	my_ace_guard  guard(_mutex);
	if (!client||!chunked||len<0)
	{
		return -1;
	}
	if (client->complete)
	{
		return 0;
	}
	if (client->isChunked)
	{
		try
		{
			if (client->content)
			{
				tmp_client_content_len=client->contentSize;
				tmp_client_content=new char[tmp_client_content_len+1];
				memset(tmp_client_content,0,tmp_client_content_len+1);
				memcpy(tmp_client_content,client->content,tmp_client_content_len);
				delete []client->content;
				client->content=0;
			}
			client->content=new char[tmp_client_content_len+len+1];
			memset(client->content,0,tmp_client_content_len+len+1);
			if (tmp_client_content)
			{
				memcpy(client->content,tmp_client_content,tmp_client_content_len);
			}	
			memcpy(client->content+tmp_client_content_len,chunked,len);
			client->contentSize+=len;
			if (tmp_client_content)
			{
				delete [] tmp_client_content;
			}
		}
		catch (...)
		{
			cout <<"out of memory"<<endl;
			::exit(1);
		}


	}
}
int Interaction_List::set_matched_client_not_chunked_content(struct clientInfo* client,char content[],int len)
{
	int minsize=0;
	my_ace_guard  guard(_mutex);
	if (!client||!content||len<0)
	{
		return -1;
	}
	if (client->isChunked)
	{
		return 0;
	}
	if (!client->complete)
	{
		minsize=len<(client->contentSize-client->currentSize)?len:(client->contentSize-client->currentSize);
		if (client->content)
		{	
			memcpy(client->content+client->currentSize,content,minsize);
		}				
		client->currentSize+=minsize;
		if (client->contentSize==client->currentSize)
		{
			client->complete=1;
		}
	}
}