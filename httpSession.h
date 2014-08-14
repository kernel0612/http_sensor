#ifndef _HTTP_SESSION_H
#define _HTTP_SESSION_H
#include "PQ.h"
#include<map>
#include<vector>
#include <time.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include "httpUtil.h"
#include "uuid/uuid.h"
#include "Pcre.h"
#include <arpa/inet.h>
#include<fstream>
#include<sys/time.h>

using namespace std;
#define  backupfile    "./data/temp.data"
enum{fromserver=11,fromclient};

struct httprequest{
	char cookie[2048];
	char url[2048];
	char method[64];
	char content[1024];  //请求正文 只取1024字节
	char host[128];
	uint32_t sip;
	uint32_t dip;
	char httpType[32];
	char refer[2048];
	char userAgent[256];
	char accept[128];
	char accEncod[128];
	unsigned short desPort;
	unsigned short srcPort;
	unsigned int cntSize;
	char requestID[64];    //uuid
	char loginAccount[64];
	char ipAddress[64];       //来源地市
	char ipBusiness[64];   //来源部门
	char appname[128];    //目标应用系统
	char parentapp[128];  //上级应用系统
	char appcode[128];    //应用系统编码
	char retstr[2048];
	char sessionid[64];
	char accLanguage[128];
	char connection[64];
	uint32_t requesttime;
	char saddr[128];
	char daddr[128];
	char echoflag;
};
struct httpresponse{
	char contentType[128];
	char resCode[128];
	char requestID[64];  //uuid
	char responseID[64];  //uuid
	char date[64];
	char content[65536] ;
};
struct Ntuple4
{
	uint16_t source;
	uint16_t dest;
	uint32_t saddr;
	uint32_t daddr;
	bool operator < (const Ntuple4 &k2) const {
		return memcmp(this, &k2, sizeof(Ntuple4)) < 0;
	}

	bool operator == (const Ntuple4 &k2) const {
		return (saddr == k2.saddr && daddr == k2.daddr && source == k2.source && \
			dest == k2.dest) || (saddr == k2.daddr && daddr == k2.saddr && \
			source == k2.dest && dest == k2.source);
	}
};
struct http_interaction{
	struct httprequest* request;
	struct httpresponse* response;
};
struct http_session{
	uint32_t ussid;
	char sessionid[64];
	uint32_t start_time;
	uint32_t end_time;
	char starttime[38];
	char endtime[38];
	uint32_t sip;
	uint16_t sport;
	uint32_t dip;
	uint16_t dport;
	vector<struct http_interaction*> interactions;
	char appname[256];
	char parentname[256];
	char appaccount[256];
};
class httpSession
{
public:
	httpSession(void);
	~httpSession(void);
	int init();
	int create_session(Ntuple4& key);
	int update_session(Ntuple4& key,const char* data,int datalen,int dataType);
	int destroy_session(Ntuple4& key);
	int fini();
private:
	int create_interaction(http_interaction** hi);
	int create_request(httprequest** req);
	int create_response(httpresponse** rep);
	int destroy_interaction(http_interaction* hi);
	int destroy_request(httprequest* req);
	int destroy_response(httpresponse* rep);

	int is_one_new_request(const char* data,int datalen);
	int my_uuid_generate(char out[],int len);
	int is_first_response(const char* data,int dataLen);

	string glue_original_kvalue_str(const char* url,const char* cookie,const char*  host
		,const char*  requestMethod,const char* postpayload,const char* responsecode );

	int insert_value_into_requesttable(httprequest* req,http_session* session);
	int insert_value_into_responsetable(httpresponse* rep);
private:
	PQ*  _out_session_conn;
	std::map<Ntuple4,struct http_session*> _session_table;
	httpUtil _httputil;
	Pcre _pcre;
	fstream out;

};
#endif
