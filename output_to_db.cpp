#include "output_to_db.h"


output_to_db::output_to_db(void)
{
}


output_to_db::~output_to_db(void)
{
}
int output_to_db::init()
{
	int ret=0;
	ret=_srvdb.open("srvInfo.db",DB_BTREE);
	if (ret!=0)
	{
		return ret;
	}
	ret=_cltdb.open("cltInfo.db",DB_BTREE);
	if (ret!=0)
	{
	    return ret;
	}
	_flag=1;
	return 0;
}
int output_to_db::fini()
{
	int ret=0;
	ret=_srvdb.close();
	if (ret!=0)
	{
		return ret;
	}
	ret=_cltdb.close();
	if (ret!=0)
	{
		return ret;
	}
	_flag=0;
	return 0;
}
int output_to_db::process(struct outputSerInfo* srv , struct outputCliInfo* clt)
{
	if (!srv||!clt||!_flag)
	{
		return -1;
	}
	int ret=0;
	ret=_srvdb.put(srv->requestID,strlen(srv->requestID),(void*)srv,sizeof(struct outputSerInfo));
	if (ret!=0)
	{
		return ret;
	}
	ret=_cltdb.put(clt->responseID,strlen(clt->responseID),(void*)clt,sizeof(struct outputCliInfo));
	return ret;
}