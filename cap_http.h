#ifndef CAP_HTTP_H
#define CAP_HTTP_H
#include "Interaction_List.h"
#include "my_ace_guard.h"
#include "my_berkeleyDBbased_fifo.h"
#include "my_ringbuffbased_fifo.h"
#include "ace/Thread_Manager.h"
//#include "nids.h"
//#include "uuid/uuid.h"
//#include "zlib.h"

class cap_http
{
public:
	cap_http(void);
	
	int init();
	int run();
	void exit();
	void register_tcp(void(*p));

	friend void http_protocol_callback(struct tcp_stream *tcp_http_connection, void **param);

	static cap_http* get_instance();
	void parse_server_data(char content[], int number,char saddr[],char daddr[],unsigned short sport,unsigned short dport);
	void parse_client_data(char content[], int number,char saddr[],char daddr[],unsigned short sport,unsigned short dport);
	my_fifo<struct CapContent>* get_capContents_fifo();
	static void destroy_instance();
    void create_thread(unsigned int n);
private:
	~cap_http(void);
	
	static void parse_content(void* args);
	int create_one_interaction(struct oneInteraction** one);
	int init_request_interaction(struct oneInteraction* pinter,char request[],char src[],char des[],unsigned short 
		sport,unsigned short dport);
	int extract_chunked_data(char entity[],int number,int inLackNumber,int isContinued,char** output,int& outlen,int &complete,int &outLackNum);
	unsigned int my_min(unsigned int len1,unsigned len2);

	int my_uuid_generate(char out[],int len);
	//int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
	int replace_str(char *sSrc, char *sMatchStr, char *sReplaceStr);
private:
    static  ACE_Thread_Mutex _mutex;
	static cap_http* _instance;
	my_fifo<struct CapContent>* _capContents_fifo;
	Interaction_List _interactons;
	ACE_Thread_Mutex _clientinfo_mutex;
//	ACE_Thread_Mutex _cap_content_block_mutex;
	int _quit;
};

#endif