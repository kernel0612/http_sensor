#include "httpSession.h"


httpSession::httpSession(void)
{
	_out_session_conn=0;
}


httpSession::~httpSession(void)
{
	fini();
}
int httpSession::create_session(Ntuple4& key){
	http_session* session=0;
	try
	{
		session=new http_session;
	}
	catch (...)
	{
		printf("new session fatal error \n");
		exit(-1);
	}
	memset(session,0,sizeof(http_session));
	time_t tv;
	time(&tv);
	//struct timeval tv; 
	//gettimeofday(&tv,NULL);
	//long long longtime=(long)tv.tv_sec*1000;
	//cout <<"start time---------------------------------------:"<<tv<<endl;

	//strncpy(session->starttime,ctime(&tv),37);
	session->start_time=tv;
	my_uuid_generate(session->sessionid,64);
	static uint32_t sid=0;
	session->ussid=sid;
	sid++;
	session->sip=key.saddr;
	session->sport=key.source;
	session->dip=key.daddr;
	session->dport=key.dest;
	char saddr[64]={0};
	char daddr[64]={0};
	strcpy(saddr,inet_ntoa(*((struct in_addr*) &(key.saddr))));
	strcpy(daddr,inet_ntoa(*((struct in_addr*) &(key.daddr))));
	_session_table.insert(make_pair(key,session));
	cout <<"sip:"<<session->sip<<"sport:"<<session->sport<<"dip:"<<session->dip<<"dport:"<<session->dport;
	cout <<"seesion start time:"<<session->starttime<<endl;
	//char *sql="copy nms_traffic from stdin";
	//char format_buff[65536]={0};
	//snprintf(format_buff, 65536 - 1, "%s\t%s\t%s\t%d\t%d\t%d\t%d\t%s\t%s\t%s\n",
	//	session->sessionid,session->starttime ,session->endtime,session->sip,session->sport,session->dip,
	//	session->dport,session->appname,session->parentname,session->appaccount);
	//int len=strlen(format_buff);
	//_out_session_conn->copy(sql,format_buff,len);

	char sql[65536]={0};
	snprintf(sql, 65536 - 1, "insert into nms_traffic values('%s',%d,NULL,'%s',%d,'%s',%d,NULL,NULL,NULL);",
			session->sessionid/*session->ussid*/,/*session->starttime*/session->start_time ,/*session->sip*/saddr,session->sport,daddr,
			session->dport);
	if(_out_session_conn->exec((const char*)sql)<0){
		printf("insert data failed \n");
		out<<sql<<endl;
		//back up
	}
	return 0;
	

}
int httpSession::update_session(Ntuple4& key,const char* data,int datalen,int dataType){
	if(!data||datalen<=0){
		return -1;
	}
	std::map<Ntuple4,struct http_session*>::iterator sessionit=_session_table.find(key);
	if(sessionit==_session_table.end()){
		return -1;
	}
	vector<struct http_interaction*>& his=sessionit->second->interactions;
	if(dataType==fromserver){   //response
		if(is_first_response(data,datalen)){
			vector<struct http_interaction*>::reverse_iterator interit=his.rbegin();
			if(interit!=his.rend()){
				my_uuid_generate((*interit)->response->responseID,64);
				memcpy((*interit)->response->requestID,(*interit)->request->requestID,64);
				_httputil.parse_response(data,datalen);
				//string& responseCode=_httputil.get_response_code();
				printf("here we got response");
				memcpy((*interit)->response->resCode,_httputil.get_response_code().c_str(),std::min(128-1,(int)(_httputil.get_response_code().length())));
				string glue_str=glue_original_kvalue_str((*interit)->request->url,(*interit)->request->cookie,
					(*interit)->request->host,(*interit)->request->method,(*interit)->request->content,(*interit)->response->resCode);
				memcpy((*interit)->request->retstr,glue_str.c_str(),std::min(2048-1,(int)(glue_str.length())));
				char sql[4096]={0};
				snprintf(sql, 4096 - 1, "update nms_traffic_request set request_protocol_str='%s' where request_id='%s';",
					glue_str.c_str(),(*interit)->request->requestID);
				if(_out_session_conn->exec((const char*)sql)<0){
					printf("update request_protocol_str failed \n");
				}
				
			}
		}
	}
	else if(dataType==fromclient){  //request
		if(is_one_new_request(data,datalen)){
			struct http_interaction* hi=0;
			struct httprequest* req=0;
			struct httpresponse* rep=0;
			time_t tv;
			time(&tv);
			/*		struct timeval tv; 
			gettimeofday(&tv,NULL);
			long long longtime=(long)tv.tv_sec*1000;*/
		
			if(create_interaction(&hi)<0){
				return -1;
			}
			if(create_request(&req)<0){
				return -1;
			}
			if(create_response(&rep)<0){
				return -1;
			}
			hi->request=req;
			hi->response=rep;
			hi->request->requesttime=tv;
			strcpy(hi->request->saddr,inet_ntoa(*((struct in_addr*) &(key.saddr))));
			strcpy(hi->request->daddr,inet_ntoa(*((struct in_addr*) &(key.daddr))));
			if(_httputil.parse_request(data,datalen)<0){
				printf("httpUtil parse request failed \n");
				return -1;
			}
			if(!_httputil.get_request_url().empty()){
				memcpy(hi->request->url,_httputil.get_request_url().c_str(),std::min(2048-1,(int)(_httputil.get_request_url().length())));
			}
			if(!_httputil.get_request_cookie().empty()){
				memcpy(hi->request->cookie,_httputil.get_request_cookie().c_str(),std::min(2048-1,(int)(_httputil.get_request_cookie().length())));
			}
			if(!_httputil.get_request_payload().empty()){
				memcpy(hi->request->content,_httputil.get_request_payload().c_str(),std::min(1024-1,(int)(_httputil.get_request_payload().length())));
			}	
			memcpy(hi->request->method,_httputil.get_request_method().c_str(),std::min(64-1,(int)(_httputil.get_request_method().length())));
			memcpy(hi->request->userAgent,_httputil.get_request_userAgent().c_str(),std::min(64-1,(int)(_httputil.get_request_userAgent().length())));
			memcpy(hi->request->host,_httputil.get_request_host().c_str(),std::min(128-1,(int)(_httputil.get_request_host().length())));
			hi->request->cntSize=_httputil.get_request_payload().length();
			hi->request->sip=key.saddr;
			hi->request->dip=key.daddr;
			hi->request->srcPort=key.source;
			hi->request->desPort=key.dest;
			my_uuid_generate(hi->request->requestID,64);
			his.push_back(hi);
			if(!_httputil.get_request_payload().empty()){
				//parse regex
				cout <<"sip:"<<key.saddr<<"sport:"<<key.source<<"dip:"<<key.daddr<<"dport:"<<key.dest;
				cout <<"seesion  update"<<endl;
				cout <<_httputil.get_request_payload()<<endl;
				string ruleName("getaccount");
				string pattern("account=\\w+");
				_pcre.AddRule(ruleName,pattern);
				vector<MatchResult> result=_pcre.MatchAllRule(_httputil.get_request_payload().c_str());
				if(result.size()>0){
					cout <<"success extract account:"<<hi->request->loginAccount<<endl;
					memcpy(hi->request->loginAccount,result[0].value[0].c_str(),64-1);
					memcpy(sessionit->second->appaccount,hi->request->loginAccount,256-1);
					//char sql[4096]={0};
					//snprintf(sql, 4096 - 1, "update nms_traffic set end_time=%d, app_account='%s' where Session_id='%s';",
					//	it->second->end_time,it->second->appaccount,it->second->sessionid/*it->second->ussid*/);
				}
				else{
					cout <<"no aacount or extract failed"<<endl;
				}
				//cout <<
			}
			insert_value_into_requesttable(hi->request,sessionit->second);

		}

	}
	else{
		return -1;
	}
	return 0;
}

int httpSession::destroy_session(Ntuple4& key){
	std::map<Ntuple4,struct http_session*>::iterator it=_session_table.find(key);
	if(it==_session_table.end()){
		return -1;
	}
	time_t tv;
	time(&tv);
	strncpy(it->second->endtime,ctime(&tv),37);
	it->second->end_time=tv;
	char sql[4096]={0};
	snprintf(sql, 4096 - 1, "update nms_traffic set end_time=%d, app_account='%s' where Session_id='%s';",
		/*it->second->endtime*/it->second->end_time,it->second->appaccount,it->second->sessionid/*it->second->ussid*/);
	int len=strlen(sql);
	if(_out_session_conn->exec((const char*)sql)<0){
		//proc backup
		printf("update data failed \n");
		out<<sql<<endl;
	}
	cout <<"sip:"<<key.saddr<<"sport:"<<key.source<<"dip:"<<key.daddr<<"dport:"<<key.dest;
	cout <<"seesion end time:"<<it->second->endtime<<endl;
	vector<struct http_interaction*>::iterator interit=
	it->second->interactions.begin();
	for(;interit!=it->second->interactions.end();++interit){
		destroy_request((*interit)->request);
		destroy_response((*interit)->response);
		destroy_interaction(*interit);
	}
	_session_table.erase(it);
	return 0;


}
int httpSession::init(){
	try{
		if(!_out_session_conn)
			_out_session_conn=new PQ("192.168.98.239",5432,"sensor","postgresql","");
	}
	catch(...){
		if(!_out_session_conn){
			delete _out_session_conn;
			_out_session_conn=0;
		}
	}
	if(_out_session_conn){
		if(_out_session_conn->connect()<0){
			printf("database connect failed \n");
			return -1;
		}
	}
	out.open(backupfile,ios::out);
	return 0;
}
int httpSession::fini(){
	if(_out_session_conn){
		_out_session_conn->disconnect();
		delete _out_session_conn;
		_out_session_conn=0;
	}
}

int httpSession::is_one_new_request(const char* data,int datalen){
	if(!data||datalen<=0){
		return -1;
	}
	char* pheadbuf=0;
	int  headlen=0;
	char* presMethod=0;
	const char* pheadEnd=strstr(data,"\r\n\r\n");
	if(pheadEnd){
		headlen=pheadEnd-data+4;
		if(headlen<=0){
			return -1;
		}
		pheadbuf=new char[headlen+1];
		if(!pheadbuf){
			return -1;
		}
		memset(pheadbuf,0,headlen+1);
		memcpy(pheadbuf,data,headlen);
		if(strstr(pheadbuf,"GET")||strstr(pheadbuf,"POST")){
			delete [] pheadbuf;
			return 1;
		}
		delete [] pheadbuf;
	}
	else{
		return 0;
	}
	return 0;
}
int httpSession::is_first_response(const char* data,int dataLen){
	if(!data||dataLen<=0){
		return -1;
	}
	char* pheadbuf=0;
	int  headlen=0;
	const char* presMethod=0;
	const char* pheadEnd=strstr(data,"\r\n\r\n");
	if(pheadEnd){
		headlen=pheadEnd-data+4;
		if(headlen<=0){
			return -1;
		}
		pheadbuf=new char[headlen+1];
		if(!pheadbuf){
			return -1;
		}
		memset(pheadbuf,0,headlen+1);
		memcpy(pheadbuf,data,headlen);
		if(strstr(pheadbuf,"HTTP/")){
			delete [] pheadbuf;
			return 1;
		}
		delete [] pheadbuf;
	}
	else{
		return 0;
	}

	return 0;
}
int httpSession::create_interaction(http_interaction** hi){
	if(!hi){
		return -1;
	}
	try{
		if(*hi==0)
		*hi=new http_interaction;
		memset(*hi,0,sizeof(http_interaction));
	}
	catch(...){
		printf("new interaction fatal error \n");
		exit(0);
	}
	return 0;
}
int httpSession::create_request(httprequest** req){
	if(!req){
		return -1;
	}
	try{
		if(*req==0)
			*req=new httprequest;
		memset(*req,0,sizeof(httprequest));
	}
	catch(...){
		printf("new httprequest fatal error \n");
		exit(-1);
	}
	return 0;
}
int httpSession::create_response(httpresponse** rep){
	if(!rep){
		return -1;
	}
	try{
		if(*rep==0)
			*rep=new httpresponse;
		memset(*rep,0,sizeof(httpresponse));
	}
	catch(...){
		printf("new httpresponse fatal error \n");
		exit(-1);
	}
	return 0;
}
int httpSession::destroy_interaction(http_interaction* hi){
	if(!hi){
		return -1;
	}
	delete hi;
	hi=0;
	return 0;
}
int httpSession::destroy_request(httprequest* req){
	if(req){
		delete req;
		req=0;
	}
	return 0;
}
int httpSession::destroy_response(httpresponse* rep){
	if(rep){
		delete rep;
		rep=0;
	}
	return 0;
}
int httpSession::my_uuid_generate(char out[],int len)
{
	uuid_t uid;
	if (!out||len<36)
	{
		return -1;
	}
	uuid_generate(uid);
	uuid_unparse(uid,out);
	return 0;
}

string httpSession::glue_original_kvalue_str(const char* url,const char* cookie,const char*  host
	,const char*  requestMethod,const char* postpayload,const char* responsecode ){
	string value;
	value.append("Request URL:");
	value.append(url);
	value.append("@");
	value.append("Request Method:");
	value.append(requestMethod);
	value.append("@");
	value.append("Status Code:");
	value.append(responsecode);
	value.append("@");
	value.append("Host:");
	value.append(host);
	value.append("@");
	value.append("Post Payload:");
	value.append(postpayload);
	return value;
}

int httpSession::insert_value_into_requesttable(httprequest* req,http_session* session){
	if(!req||!session){
		return -1;
	}
	string appname("pmo");
	string parentapp("ultrapower");
	string loginaccount(req->loginAccount);
	if(loginaccount.empty()){
		loginaccount="NULL";
	}
	string method(req->method);
	string refer(req->refer);
	string userAgent(req->userAgent);
	if(userAgent.empty()){
		userAgent="NULL";
	}
	string host(req->host);
	string cookie(req->cookie);
	string x_look_f;
	string content(req->content);
	if(content.empty()){
		content="NULL";
	}
	string requestID(req->requestID);
	if(requestID.empty()){
		requestID="NULL";
	}
	string retstr(req->retstr);
	if(retstr.empty()){
		retstr="NULL";
	}
	string url(req->url);
	string acclanguage(req->accLanguage);
	string httpType(req->httpType);
	char sql[65536]={0};
	snprintf(sql, 65536 - 1, "insert into nms_traffic_request values(%d,'%s',%d,'%s',%d,'%s','%s','%s','%s',NULL,NULL,'%s','%s','%s',NULL,'%s','%s','%s','%s',NULL,NULL,NULL,NULL,NULL,NULL,'ip','busi','dessys','%s');",
		req->requesttime,req->saddr,req->srcPort,req->daddr,req->desPort,appname.c_str(),parentapp.c_str(),
		loginaccount.c_str(),req->method,userAgent.c_str(),req->host,req->cookie,
		content.c_str(),req->requestID,retstr.c_str(),req->url,session->sessionid);

	_out_session_conn->exec((const char*)sql);
	return 0;
}
int httpSession::insert_value_into_responsetable(httpresponse* rep){

	return 0;
}