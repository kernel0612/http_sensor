#include "my_berkeleyDBbased_fifo.h"
#include "my_ringbuffbased_fifo.h"
#include "commondef.h"
#include "outputdef.h"
#include "my_berkeleyDB.h"
#include "Interaction_List.h"
#include "ace/Thread_Manager.h"
#include "cap_http.h"
#include "TimeOut_Handler.h"

//void get_size(void* arg)
//{
//	my_fifo<struct CapContent>* fifo=(my_fifo<struct CapContent>*)arg;
//	while(1)
//	{
//		printf("size:%d\n",fifo->get_element_size());
//	}
//}
//int i=0;
//void prod(void* arg)
//{
//	my_fifo<struct cap_content_block>* fifo=(my_fifo<struct cap_content_block>*)arg;
//	struct cap_content_block cons;
//	ACE_Time_Value sleepTime(0,5000);
//	int ret=0;
//	while(1)
//	{
//		memset(&cons,0,sizeof(struct cap_content_block));
//		cons.Sport=i;
//		if (ret=fifo->push_back(cons)!=0)
//		{
//			printf("push_back error\n");
//			if (ret==1)
//			{
//				printf("timeout\n");
//				break;
//			}
//		}
//		i++;	
//		ACE_OS::sleep(sleepTime);
//	}
//}
//void cons(void* arg)
//{
//	my_fifo<struct cap_content_block>* fifo=(my_fifo<struct cap_content_block>*)arg;
//	struct cap_content_block content;
//	ACE_Time_Value sleepTime(0,50000);
//	int ret=0;
//	while(1)
//	{
//		memset(&content,0,sizeof(content));
//		if (ret=fifo->pop_front(content)==0)
//		{
//			printf("%d\n",content.Sport);
//		}
//		else if(ret==1)
//		{
//			printf("pop_front error timeout\n");
//			break;
//		}
//	    ACE_OS::sleep(sleepTime);
//	}
//}

int main(int argc,char** argv)
{

	//my_fifo<struct cap_content_block>* fifo=new my_berkeleyDBbased_fifo<struct cap_content_block>(1000);
	//if (fifo->init()!=0)
	//{
	//	delete fifo;
	//	system("pause");
	//	return 0;
	//}

	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)prod,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->wait();

	//delete fifo;

	//proc_output_interaction* p=new output_to_file;
	//if (p->init()!=0)
	//{
	//	cout<<"init error"<<endl;
	//	return -1;
	//}
	//struct outputSerInfo osi;
	//struct outputCliInfo oci;
	//memset(&osi,0,sizeof(struct outputSerInfo));
	//memset(&oci,0,sizeof(struct outputCliInfo));
	//osi.desPort=1;
	//osi.srcPort=2;
	//memcpy(osi.requestID,"sdfsfsf",16);
	//memcpy(oci.responseID,"sdffsffe",16);
	//if (p->process(&osi,&oci)!=0)
	//{
	//	cout <<"process error"<<endl;
	//}
	//p->fini();


	cap_http* ins=cap_http::get_instance();
	if (ins->init()!=0)
	{
		cout <<"init failed"<<endl;
		return -1;
	}
	ins->register_tcp();
	ins->run();
	cap_http::destroy_instance();
	return 0;
};