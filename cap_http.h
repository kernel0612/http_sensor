#ifndef CAP_HTTP_H
#define CAP_HTTP_H
#include <vector>
#include "Interaction_List.h"
#include "my_ace_guard.h"
#include "my_berkeleyDBbased_fifo.h"
#include "my_ringbuffbased_fifo.h"
#include "ace/Thread_Manager.h"
#include "proc_capCnt_block.h"
#include "output_to_file.h"
#include "output_to_db.h"
#include "nids.h"
#include "uuid/uuid.h"
#include "zlib.h"

class cap_http
{
public:
	cap_http(void);
	
	int init();
	int run();
	void exit();
	void register_tcp();

	friend void http_protocol_callback(struct tcp_stream *tcp_http_connection, void **param);

	static cap_http* get_instance();
	static void destroy_instance();


private:
	~cap_http(void);
	my_fifo<struct cap_content_block>* get_capContents_fifo();
	proc_capCnt_block* get_proc_capCnt_block();
	Interaction_List& get_interaction_list();
	void parse_server_data(char content[], int number,char saddr[],char daddr[],unsigned short sport,unsigned short dport);
	void parse_client_data(char content[], int number,char saddr[],char daddr[],unsigned short sport,unsigned short dport);
	int run_parse_content_loop();
	int run_ace_event_loop();
	int run_output_loop();
	int run_nids_cap_loop();
	int run_monitor_thread();
	static void parse_content(void* args);
	static void handle_ace_event(void* args);
	static void output_interaction_data(void* args);
	static void nids_cap_loop(void* args);
	static void monitor_thread(void* args);
	int create_one_interaction(interaction** one);
	int init_request_interaction(interaction* pinter,char request[],char src[],char des[],unsigned short 
		sport,unsigned short dport);
	int extract_chunked_data(char entity[],int number,char* output,int inlen,int& outlen,int &complete);
	unsigned int my_min(unsigned int len1,unsigned int len2);

	int my_uuid_generate(char out[],int len);
	int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
	int replace_str(char *sSrc, char *sMatchStr, char *sReplaceStr);
	int print_one_interaction(interaction* in);

	int init_proc_interactions();
	int process_output_method(struct outputSerInfo* srv , struct outputCliInfo* clt);

	cap_http(const cap_http& );
	cap_http& operator =(const cap_http& );
private:
	ACE_Select_Reactor _select_reactor;
	ACE_Reactor _reactor;
    static  ACE_Thread_Mutex _mutex;
	static cap_http* _instance;
	my_fifo<struct cap_content_block>* _capContents_fifo;
	Interaction_List _interactons;
	ACE_Thread_Mutex _clientinfo_mutex;
	int _quit;
	proc_capCnt_block* _pccb;
	//TimeOut_Handler* th;
	//TimeOut_Handler* th1;
	//TimeOut_Handler* th2;

	vector<proc_output_interaction*> _proc_outputs;
	
};

#endif