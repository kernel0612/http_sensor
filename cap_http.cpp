#include "cap_http.h"

cap_http* cap_http::_instance=0;
ACE_Thread_Mutex cap_http::_mutex;
cap_http::cap_http(void):_capContents_fifo(new my_berkeleyDBbased_fifo<struct cap_content_block>())
	,_pccb(new proc_capCnt_block),_quit(0),_reactor(&_select_reactor)
{
	
}
cap_http::~cap_http(void)
{
}

int cap_http::init()
{
	int ret=0;
	//ret=nids_init();
	ret=get_capContents_fifo()->init();
	cout <<"init()"<<endl;
	th=new TimeOut_Handler(0,5,&_reactor);
	th1=new TimeOut_Handler(0,6,&_reactor);
	th2=new TimeOut_Handler(0,7,&_reactor);
	return ret;
}
int cap_http::run()
{
   int ret=0;
   run_ace_event_loop();
   run_nids_cap_loop();
   run_output_loop();
   run_parse_content_loop();
   run_monitor_thread();
   ACE_Thread_Manager::instance()->wait();
   return ret;
}

void cap_http::exit()
{
	//nids_exit();
	_quit=1;
	_reactor.end_reactor_event_loop();
	_interactons.disabled();
	get_capContents_fifo()->disabled();
}
void cap_http::register_tcp(void(*p))
{
	//nids_register_tcp(p);
}

void http_protocol_callback(struct tcp_stream *tcp_http_connection, void **param)
{
	cap_http* ins=cap_http::get_instance();
	interaction* intern=0;
	char address_content[1024];
	char saddr[32];
	char daddr[32];
	unsigned short sport;
	unsigned short dport;
	//struct tuple4 ip_and_port = tcp_http_connection->addr;
	//strcpy(saddr,inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
	//strcpy(daddr,inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
	//sport=ip_and_port.source;
	//dport=ip_and_port.dest;
	//
	//unsigned int blockNum=0;
	//unsigned int writebegin=0;
	//unsigned int currBlockSize=0;

	//if (tcp_http_connection->nids_state == NIDS_JUST_EST)
	//{
	//	if (tcp_http_connection->addr.dest != 80)
	//	{
	//		return ;
	//	}
	//	tcp_http_connection->client.collect++; 
	//	tcp_http_connection->server.collect++; 
	//	return ;
	//}
	//if (tcp_http_connection->nids_state == NIDS_CLOSE)
	//{
	intern=ins->get_interaction_list().get_matched_interaction_m(saddr,daddr,sport);
	if (intern)
	{
		intern->set_interactin_status(INTERACTION_CLOSE);
	}	
	//	return ;
	//}
	//if (tcp_http_connection->nids_state == NIDS_RESET)
	//{
	intern=ins->get_interaction_list().get_matched_interaction_m(saddr,daddr,sport);
	if (intern)
	{
		intern->set_interactin_status(INTERACTION_RESET);
	}	
	//	return ;
	//}
	//if (tcp_http_connection->nids_state == NIDS_DATA)
	//{
	//	struct half_stream *hlf;
	//	if (tcp_http_connection->client.count_new)
	//	{
	//		struct cap_content_block block;
	//		memset(&block,0,sizeof(struct cap_content_block));
	//		hlf = &tcp_http_connection->client;	
	//		memcpy(block.Src,saddr,32);
	//		memcpy(block.Des,daddr,32);
	//		block.Sport=sport;
	//		block.Dport=dport;
	//		block.type=CLIENT_CNT;
	//		blockNum=(hlf->count_new/CAP_CONTENT_BLOCK_SIZE)+(hlf->count_new%CAP_CONTENT_BLOCK_SIZE==0?0:1);
	//		for (int i=1;i<=blockNum;i++)
	//		{
	//			currBlockSize=i<blockNum?(CAP_CONTENT_BLOCK_SIZE):(hlf->count_new-(blockNum-1)*CAP_CONTENT_BLOCK_SIZE);
	//			memcpy(block.CntBlock,hlf->data+writebegin,currBlockSize);
	//			writebegin+=CAP_CONTENT_BLOCK_SIZE;
	//			ins->get_capContents_fifo()->push_back(block);
	//		}

	//	}
	//	if (tcp_http_connection->server.count_new)
	//	{
	//		struct cap_content_block block;
	//		memset(&block,0,sizeof(struct cap_content_block));
	//		hlf = &tcp_http_connection->server;       
	//		memcpy(block.Src,saddr,32);
	//		memcpy(block.Des,daddr,32);
	//		block.Sport=sport;
	//		block.Dport=dport;
	//		block.type=SERVER_CNT;
	//		blockNum=(hlf->count_new/CAP_CONTENT_BLOCK_SIZE)+(hlf->count_new%CAP_CONTENT_BLOCK_SIZE==0?0:1);
	//		for (int i=1;i<=blockNum;i++)
	//		{
	//			currBlockSize=i<blockNum?(CAP_CONTENT_BLOCK_SIZE):(hlf->count_new-(blockNum-1)*CAP_CONTENT_BLOCK_SIZE);
	//			memcpy(block.CntBlock,hlf->data+writebegin,currBlockSize);
	//			writebegin+=CAP_CONTENT_BLOCK_SIZE;
	//			ins->get_capContents_fifo()->push_back(block);
	//		}
	//	}
	//}
}
my_fifo<struct cap_content_block>* cap_http::get_capContents_fifo()
{
	return _capContents_fifo;
}
cap_http* cap_http::get_instance()
{
	if (_instance==0)
	{
		my_ace_guard  guard(_mutex);
		if (_instance==0)
		{
			_instance=new cap_http;
		}
	}
	return _instance;
}
void cap_http::destroy_instance()
{
	if (_instance)
	{
		delete _instance;
	}
}
void cap_http::parse_server_data(char content[], int number,char saddr[],char daddr[],
	unsigned short sport,unsigned short dport)
{
	interaction* pinter=0;
	struct serverInfo* serinfo=0;          
	char temp[MAX_BUFF_SIZE]={0};
	char* seg={0};
	int i=0;
	int k=0;
	int j=0;
	int entity_index=0;
	char entity_content[REQUEST_CONTENT_BUF_SIZE]={0};
	int ret;
	//-------------------------------------------------
	for (i=0;i<number;i++)
	{
		if ((i+1)<number)
		{
			if (content[i]=='\r'&&content[i+1]=='\n')
			{
				memset(temp,0,sizeof(temp));
				memcpy(temp,content+i-k,my_min(k,MAX_BUFF_SIZE-1));
				k=0;
				if (strstr(temp, "GET")||strstr(temp,"POST"))  //方法暂时只取两种GET POST
				{
					ret=create_one_interaction(&pinter);
					if (ret==0)
					{
						init_request_interaction(pinter,temp,saddr,daddr,sport,dport);
					}
				}
				if (strstr(temp, "Accept:"))
				{
					seg=(temp + strlen("Accept: "));
					if (serinfo)
					{
						strncpy(serinfo->accept,seg,my_min(strlen(seg),ACCEPT_BUF_SIZE-1));
					}		

				}
				if (strstr(temp, "Referer"))
				{
					seg=(temp+strlen("Referer: "));
					if (serinfo)
					{
						strncpy(serinfo->refer,seg,my_min(strlen(seg),REFERER_BUF_SIZE-1));
					}		
				}
				if (strstr(temp, "Accept-Encoding"))
				{
					//  printf("Accept-Encoding is:%s\n", temp + strlen("Accept-Encoding:"));
					//gzip编码  需要解压
					seg=(temp+strlen("Accept-Encoding: "));
					if (serinfo)
					{
						strncpy(serinfo->accEncod,seg,my_min(strlen(seg),ACCEPTENCODE_BUF_SIZE-1));
					}
				}
				if (strstr(temp, "User-Agent"))
				{
					seg=(temp + strlen("User-Agent: "));
					if (serinfo)
					{
						strncpy(serinfo->userAgent,seg,my_min(strlen(seg),USERAGENT_BUF_SIZE-1));
					}
				}
				if (strstr(temp, "Host"))
				{
					seg=(temp + strlen("Host: "));
					if (serinfo)
					{
						strncpy(serinfo->host,seg,my_min(strlen(seg),HOST_BUF_SIZE-1));
						replace_str(serinfo->url,serinfo->des,serinfo->host);
					}
				}
				if (strstr(temp, "Cookie"))
				{
					//printf("Cookie is:%s\n", temp + strlen("Cookie: "));
					//暂为空
				}
				//temp operation end

				if ((i+3)<number)               //正文部分
				{
					if (content[i]=='\r'&&content[i+1]=='\n'&&content[i+2]=='\r'&&content[i+3]=='\n')  //
					{
						if (number==i+4)
						{
	                       //no content
							break;
						}
						memcpy(entity_content,content+i+4,my_min(number-i-4,REQUEST_CONTENT_BUF_SIZE-1));
						if (serinfo)
						{
							//strncpy(serinfo->content,entity_content,sizeof(serinfo->content)/sizeof(serinfo->content[0]));//1024
							memcpy(serinfo->content,entity_content,REQUEST_CONTENT_BUF_SIZE-1);
						}
					    break;
					}
				}

				i+=1;
			}
			else
			{
				++k;	
				continue;
			}
		}

	}
	//----------------------------------------------
	if (pinter)
	{
        _interactons.put(pinter);
	}
}
void cap_http::parse_client_data(char content[], int number,char saddr[],char daddr[],
	unsigned short sport,unsigned short dport)
{
	struct clientInfo* client=0;
	interaction* intern=0;
	TimeOut_Handler *th=0;
	char temp[MAX_BUFF_SIZE]={0};
	int i=0;
	int k=0;
	int j=0;
	char entity_content[MAX_BUFF_SIZE]={0};
	int chunkflag=0;
	int complete=0;
	unsigned int contentSize=0;
	unsigned int currentSize=0;
	char* seg=0;
	char resCode[RESPONSECODE_BUF_SIZE]={0};
	char date[DATE_BUF_SIZE]={0};
	char responseID[RESPONSEID_BUF_SIZE]={0};
	char contentType[CONTENTTYPE_BUF_SIZE]={0};
	char* pmalloc=0;
	int continued=0;
	int writeLen=0;
	int extract_chunk_ret=0;
	intern=_interactons.get_matched_interaction_m(saddr,daddr,sport);
	if (!intern)
	{
		return;
	}
	th=intern->get_timeout_handler();
	if (th)
	{
		th->cancel_timer();
	}
	client=intern->get_client_info();
    //set filter

	//set filter
	if (!client)
	{
		return;
	}
	//-----------------------------------------------------------

	if (number<4)
	{
		writeLen=my_min(number,MAX_BUFF_SIZE-1);
	    memcpy(entity_content,content,writeLen);
	    continued=1;
	}
	if (number>=4)
	{
		if(content[0] != 'H' && content[1] != 'T' && content[2] != 'T' && content[3] != 'P')   //entity continued....
		{
			writeLen=my_min(number,MAX_BUFF_SIZE-1);
			memcpy(entity_content,content,writeLen);
			continued=1;
		}
		else if (content[0] == 'H' && content[1] == 'T' && content[2] == 'T' && content[3] == 'P')
		{
			for (i=0;i<number;i++)
			{
				if ((i+1)<number)
				{
					if (content[i]=='\r'&&content[i+1]=='\n')
					{
						memset(temp,0,sizeof(temp));
						memcpy(temp,content+i-k,my_min(k,MAX_BUFF_SIZE-1));
						k=0;
						//temp operation
						if (strstr(temp,"HTTP"))
						{
							strncpy(resCode,temp,my_min(strlen(temp),RESPONSECODE_BUF_SIZE));
							//my_uuid_generate(client->responseID,64);
						}
						if (strstr(temp, "Date"))
						{
							seg=(temp+strlen("Date: "));
							strncpy(date,seg,my_min(strlen(seg),DATE_BUF_SIZE-1));
						}
						if (strstr(temp, "Content-Length"))
						{				
							seg=(temp+strlen("Content-Length: "));
							contentSize=strtol(seg,NULL,10);           //取得content-length
							if (contentSize==0)
							{
								complete=1;
							}	
							else
							{
								try
								{
									 if (contentSize>65535)
									 {
										 contentSize=65535;
									 }
									 pmalloc=new char[contentSize+1];
									 memset(pmalloc,0,contentSize+1);
								}
								catch(...)
								{
									delete [] pmalloc;
									pmalloc=0;
								}                                
							}
						}
						if (strstr(temp, "Content-Type"))
						{
							seg=(temp+strlen("Content-Type: "));
							strncpy(contentType,seg,my_min(strlen(seg),CONTENTTYPE_BUF_SIZE-1));
						}
						if (strstr(temp, "Transfer-Encoding"))
						{
							seg=(temp+strlen("Transfer-Encoding: "));
							if (strstr(seg,"chunked"))
							{
								chunkflag=1;
							}
						}
						//temp operation end
						if ((i+3)<number)
						{
							if (content[i]=='\r'&&content[i+1]=='\n'&&content[i+2]=='\r'&&content[i+3]=='\n')  //content
							{
								if (number==i+4)
								{
									//printf("no content \n");    //no content
									break;
								}		
								writeLen=my_min(number-i-4,MAX_BUFF_SIZE-1);
								memcpy(entity_content,content+i+4,writeLen);				
								break;
							}
						}

						i+=1;
					}
					else
					{
						++k;	
						continue;
					}
				}

			}
		}
	}

	int minsize=0;
	char* tmp_client_content=0;
	int tmp_client_content_len=0;
	//lock
	 //将内容写到链表的clientinfo中
	{
		my_ace_guard guard(_clientinfo_mutex);
		if (!(client->complete))
		{
			if (!continued)
			{
				if (chunkflag)
				{
					client->isChunked=1;
				}
				else
				{
					client->content=pmalloc;
					client->complete=complete;
					client->contentSize=contentSize;
				}
				memcpy(client->responseID,responseID,RESPONSEID_BUF_SIZE);
				memcpy(client->contentType,contentType,CONTENTTYPE_BUF_SIZE);
				memcpy(client->resCode,resCode,RESPONSECODE_BUF_SIZE);
				memcpy(client->date,date,DATE_BUF_SIZE);
			}

			if (client->isChunked)
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
				client->content=new char[tmp_client_content_len+writeLen+1];
				memset(client->content,0,tmp_client_content_len+writeLen+1);
				if (tmp_client_content)
				{
					memcpy(client->content,tmp_client_content,tmp_client_content_len);
				}	
				memcpy(client->content+tmp_client_content_len,entity_content,writeLen);
				client->contentSize+=writeLen;
				if (tmp_client_content)
				{
					delete [] tmp_client_content;
				}

			}
			else
			{
				if (!client->complete)
				{
					minsize=my_min(writeLen,client->contentSize-client->currentSize);
					if (client->content)
					{	
						memcpy(client->content+client->currentSize,entity_content,minsize);
					}				
					client->currentSize+=minsize;
					if (client->contentSize==client->currentSize)
					{
						client->complete=1;
					}
				}

			}


		}
	}

	//unlock
}
void cap_http::parse_content(void* args)
{
	cap_http* ins=cap_http::get_instance();
	ACE_Time_Value sleep_tv(0,5000);
	struct CapContent cap;
	struct cap_content_block  block;
	int ret=0;
	ACE_DEBUG((LM_INFO,"parse content thread begin\n"));
	while(!ins->_quit)
	{
		memset(&cap,0,sizeof(struct CapContent));
		memset(&block,0,sizeof(struct cap_content_block));
		ret=ins->get_capContents_fifo()->pop_front(block);
		if (ret==0)
		{
			if (ins->get_proc_capCnt_block()->append_block_to_proccess(block,cap)==0)
			{
				if (cap.cliHasCnt)
				{
					ins->parse_client_data(cap.cliCnt,cap.cliCntSize,cap.cliSrc,cap.cliDes,cap.cliSport,cap.cliDport);
				}
				if (cap.srvHasCnt)
				{
					ins->parse_server_data(cap.srvCnt,cap.srvCntSize,cap.srvSrc,cap.srvDes,cap.srvSport,cap.srvDport);
				}
			}

		}
		if (ret==-1)
		{
		}
		if (ret==1)
		{
			//printf("wait not empty time out\n");
			break;
		}
		//ACE_OS::sleep(sleep_tv);
	}
    ACE_DEBUG((LM_INFO,"parse content thread quit\n"));
}

int cap_http::run_parse_content_loop()
{
	return ACE_Thread_Manager::instance()->spawn_n(PARSE_CONTENT_THREAD_NUM,(ACE_THR_FUNC)parse_content,0);
}
int cap_http::run_ace_event_loop()
{
	return ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)handle_ace_event,&_reactor);
}
int cap_http::run_output_loop()
{
	return ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)output_interaction_data,0);
}
int cap_http::run_nids_cap_loop()
{
	return ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)nids_cap_loop,0);
}
int cap_http::run_monitor_thread()
{
	return ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)monitor_thread,0);
}
int cap_http::create_one_interaction(interaction** one)
{
	if (!one)
	{
		return -1;
	}	
	try
	{
		*one=new interaction;
		(*one)->set_server_info(new struct serverInfo);
		(*one)->set_client_info(new struct clientInfo);
		memset((*one)->get_client_info(),0,sizeof(struct clientInfo));
		memset((*one)->get_server_info(),0,sizeof(struct serverInfo));
		(*one)->set_interactin_status(INTERACTION_NORMAL);
		(*one)->set_timeout_handler(new TimeOut_Handler(*one,10,&_reactor));
	}
	catch (...)
	{
		if (*one)
		{
			if ((*one)->get_client_info())
			{
				delete (*one)->get_client_info();
				(*one)->set_client_info(0);
			}
			if ((*one)->get_server_info())
			{
				delete (*one)->get_server_info();
				(*one)->set_server_info(0);
			}
			delete (*one);
			*one=0;
		}
		return -1;
	}
	return 0;
}

int cap_http::init_request_interaction(interaction* pinter,char request[],char src[],char des[],unsigned short 
	sport,unsigned short dport)
{
	if (!pinter||!request||!des)
	{
		return -1;
	}
	struct serverInfo* serinfo=pinter->get_server_info();  
	if (!serinfo)
	{
		return -1;
	}
	char method[32]={0};
	char resource[10240]={0};
	char httptype[32]={0};
	unsigned int http_head_len=0;
	unsigned int desAddr_len=0;
	strncpy(serinfo->src,src,strlen(src));
	strncpy(serinfo->des,des,strlen(des));
	serinfo->srcPort=sport;
	serinfo->desPort=dport;
	sscanf(request, "%s %s %s", method, resource, httptype);//分割为 GET  resource http/1.1 形式
	strncpy(serinfo->method,method,my_min(strlen(method),METHOD_BUF_SIZE)-1);
	http_head_len=strlen("http://");
	strncpy(serinfo->url,"http://",http_head_len);
	desAddr_len=strlen(serinfo->des);
	strncat(serinfo->url,serinfo->des,desAddr_len);
	strncat(serinfo->url,resource,my_min(strlen(resource),URL_BUF_SIZE-http_head_len-desAddr_len)-1);
	strncpy(serinfo->httpType,httptype,my_min(strlen(httptype),HTTPTYPE_BUFF_SIZE)-1);
	//my_uuid_generate(serinfo->requestID,64);
	return 0;
}
unsigned int cap_http::my_min(unsigned int len1,unsigned int len2)
{
	return len1<len2?len1:len2;
}
//提取chunked方式的响应正文
int cap_http::extract_chunked_data(char entity[],int number,char* output,int inlen,int& outlen,int &complete) 
{
	int i=0,j=0,k=0,chunkedLen=0,contentLen=0;
	char chunkedArr[32]={0};
	char content[MAX_BUFF_SIZE]={0};
	int client_content_len=0;
	int tmp_content_len=0;
	int minSize=0;	
	if (!output||!entity||!number||number>MAX_BUFF_SIZE)
	{
		return -1;
	}
	for (i=0;i<number;i++)
	{
		if (i+1<number)
		{
			if (entity[i]=='\r'&&entity[i+1]=='\n')
			{
				contentLen=0;
				memset(chunkedArr,0,32);
				memset(content,0,MAX_BUFF_SIZE);
				tmp_content_len=0;
				memcpy(chunkedArr,entity+(i-chunkedLen),my_min(chunkedLen,31));		
				chunkedLen=0;
				contentLen=strtol(chunkedArr,NULL,16);
				if (contentLen>0&&contentLen<=MAX_BUFF_SIZE)
				{
					k=i+2;       //跳过\r\n找到正文部分
					i+=(contentLen+2+1);  //+contenLen跳过正文转到下一段 +2 跳过\r\n +1跳过 \r forloop接着i++跳过\n
					memcpy(content,entity+k,minSize=my_min(number,contentLen));					  			  
					memcpy(output+client_content_len,content,minSize=my_min(minSize,inlen-client_content_len));
					client_content_len+=minSize;
					outlen=client_content_len;		
				}
				else if(contentLen==0)
				{
					complete=1;
					break;
				}

			}
			else
			{
				++chunkedLen;
			}
		}
		else if (i+1==number)
		{
			if (entity[i]=='0')
			{
				complete=1;
				break;
			}
		}

	}
	return 0;
}

int cap_http::my_uuid_generate(char out[],int len)
{
	//uuid_t uid;
	//if (!out||len<36)
	//{
	//	return -1;
	//}
	//uuid_generate(uid);
	//uuid_unparse(uid,out);
	return 0;
}

//int cap_http::httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
//{
//	int err = 0;
//	z_stream d_stream = {0}; /* decompression stream */
//	static char dummy_head[2] =
//	{
//		0x8 + 0x7 * 0x10,
//		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
//	};
//	d_stream.zalloc = (alloc_func)0;
//	d_stream.zfree = (free_func)0;
//	d_stream.opaque = (voidpf)0;
//	d_stream.next_in  = zdata;
//	d_stream.avail_in = 0;
//	d_stream.next_out = data;
//	if(inflateInit2(&d_stream,47 ) != Z_OK) return -1;
//	printf("ds.total_out:%d,ndata:%d,ds.total_in:%d,nzdata:%d\n",d_stream.total_out,*ndata,d_stream.total_in,nzdata);
//	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
//		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
//		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
//		if(err != Z_OK )
//		{
//			if(err == Z_DATA_ERROR)
//			{
//				d_stream.next_in = (Bytef*) dummy_head;
//				d_stream.avail_in = sizeof(dummy_head);
//				if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
//				{
//					printf("err: Z_data_error\n");
//					return -1;
//				}
//				printf("err: Z_not_OK\n");
//			}
//
//			else return -1;
//		}
//	}
//	printf("quit whileloop\n");
//	if(inflateEnd(&d_stream) != Z_OK) return -1;
//	printf("uncp successful\n");
//	*ndata = d_stream.total_out;
//	return 0;
//}

int cap_http::replace_str(char *sSrc, char *sMatchStr, char *sReplaceStr)   //替换匹配部分的字符串   只替换一次  使用host替换 ip
{
	int  StringLen;
	char caNewString[2048];

	char *FindPos = strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;

	if(FindPos)                      
	{
		memset(caNewString, 0,sizeof(caNewString));
		StringLen = FindPos -sSrc;
		strncpy(caNewString, sSrc, StringLen);                     
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));           //假设 url中不含有'\0'
		strcpy(sSrc, caNewString);
	}

	return 0;
}
proc_capCnt_block* cap_http::get_proc_capCnt_block()
{
	return _pccb;
}
Interaction_List& cap_http::get_interaction_list()
{
	return _interactons;
}

void cap_http::handle_ace_event(void* args)
{
	//cap_http* ins=cap_http::get_instance();
	//ACE_Time_Value sleep_val(0,5000);
	ACE_Reactor* reactor=(ACE_Reactor*)args;
	 ACE_DEBUG((LM_INFO,"handle_ace_event thread begin\n"));
	reactor->owner(ACE_OS::thr_self());
	reactor->run_reactor_event_loop();
	 ACE_DEBUG((LM_INFO,"handle_ace_event thread quit\n"));
}
void cap_http::output_interaction_data(void* args)
{
	cap_http* ins=cap_http::get_instance();
	ACE_Time_Value sleep_val(0,5000);
	interaction * pinter=0;
	struct serverInfo* psrv=0;
	struct clientInfo* pclt=0;
	ACE_DEBUG((LM_INFO,"output_interaction_data thread begin\n"));
	int ret=0;
	char tmpbuff[MAX_BUFF_SIZE*4]={0};
	while(!ins->_quit)
	{
		ret=ins->get_interaction_list().pop(&pinter);
		if (ret==0)
		{
			psrv=pinter->get_server_info();
			pclt=pinter->get_client_info();
			if (psrv)
			{
				
			}
			if (pclt)
			{
				pclt->isChunked;
			}

		}
		else if (ret==-1)
		{
		}
		else if (ret==1)
		{
			break;
		}
		ACE_OS::sleep(sleep_val);
	}
	ACE_DEBUG((LM_INFO,"output_interaction_data thread quit\n"));
}

void cap_http::nids_cap_loop(void* args)
{
	cap_http* ins=cap_http::get_instance();
	ACE_DEBUG((LM_INFO,"nids_cap_loop thread begin\n"));
	int ret=0;
	// ret=nids_run();
	ACE_DEBUG((LM_INFO,"nids_cap_loop thread quit\n"));
}
void cap_http::monitor_thread(void* args)
{
	cap_http* ins=cap_http::get_instance();
	ACE_DEBUG((LM_INFO,"monitor_thread begin\n"));
	ACE_OS::sleep(3);
	while(1)
	{
		ACE_DEBUG((LM_INFO,"type in 'q'to quit system\n"));
		char c=getchar();
		if (c=='q')
		{
			ins->exit();
			break;
		}
		ACE_OS::sleep(2);
	}
	ACE_DEBUG((LM_INFO,"monitor_thread quit\n"));

}