#include "output_to_db.h"


output_to_db::output_to_db(const char *ip, uint16_t pport, const char *pdbname
	, const char *puser, const char *ppasswd, int pconnect_timeout):
_pqDB(ip,pport,pdbname,puser,ppasswd,pconnect_timeout)
{
}


output_to_db::~output_to_db(void)
{
}
int output_to_db::init()
{
	if (_pqDB.connect()==0)
	{
		_flag=1;
		return 0;
	}
	return -1;
}
int output_to_db::fini()
{
	if ( _pqDB.disconnect()==0)
	{
		_flag=0;
		return 0;
	}
	return -1;
}
int output_to_db::process(struct outputSerInfo* srv , struct outputCliInfo* clt)
{
	if (!srv||!clt||!_flag)
	{
		return -1;
	}
	int ret=0;
	char* srvsql="copy serverinfo from stdin";
	char* cltsql="copy clientinfo from stdin";
	char srv_format_buff[65536]={0};
	char clt_format_buff[1024]={0};
	size_t srv_format_len=0;
	size_t clt_format_len=0;
	snprintf(srv_format_buff, 65536 - 1, "%s\t%s\t%s\t%s\t%s\t%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		srv->requestID, srv->url, srv->method, srv->host, srv->src,srv->srcPort,
		srv->des,srv->desPort,srv->httpType,srv->refer,srv->accept,srv->accEncod,srv->userAgent,srv->content,srv->cookie);
	srv_format_len = strlen(srv_format_buff);
	snprintf(clt_format_buff, 1024 - 1, "%s\t%s\t%s\t%s\t%s\t%s\n",
		clt->responseID, clt->requestID, clt->contentType, clt->date, clt->resCode, clt->content);
	clt_format_len = strlen(clt_format_buff);
	if (ret=_pqDB.copy(srvsql,srv_format_buff,srv_format_len)!=0)
	{
		cout <<"copy serverinfo error"<<endl;
		return -1;
	}
	if (ret=_pqDB.copy(cltsql,clt_format_buff,clt_format_len)!=0)
	{
		cout <<"copy clientinfo error"<<endl;
		return -1;
	}
	return ret;
}