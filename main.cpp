#include "my_berkeleyDBbased_fifo.h"
#include "my_ringbuffbased_fifo.h"
#include "commondef.h"
#include "outputdef.h"
#include "my_berkeleyDB.h"
#include "Interaction_List.h"
#include "ace/Thread_Manager.h"
#include "cap_http.h"
#include "TimeOut_Handler.h"
//#include "ace/OS.h"
void get_size(void* arg)
{
	my_fifo<struct CapContent>* fifo=(my_fifo<struct CapContent>*)arg;
	while(1)
	{
		printf("size:%d\n",fifo->get_element_size());
		//Sleep(5);
	}
}
int i=0;
void prod(void* arg)
{
	my_fifo<struct CapContent>* fifo=(my_fifo<struct CapContent>*)arg;
	struct CapContent cons;
	ACE_Time_Value sleepTime(0,5000);
	int ret=0;
	while(1)
	{
		memset(&cons,0,sizeof(struct CapContent));
		cons.srvSport=i;
		if (ret=fifo->push_back(cons)!=0)
		{
			printf("push_back error\n");
			if (ret==1)
			{
				printf("timeout\n");
				break;
			}
		}
		i++;	
		//Sleep(10);
		ACE_OS::sleep(sleepTime);
	}
}
void cons(void* arg)
{
	my_fifo<struct CapContent>* fifo=(my_fifo<struct CapContent>*)arg;
	struct CapContent content;
	ACE_Time_Value sleepTime(0,50000);
	int ret=0;
	while(1)
	{
		memset(&content,0,sizeof(content));
		if (ret=fifo->pop_front(content)==0)
		{
			printf("%d\n",content.srvSport);
		}
		else if(ret==1)
		{
			printf("pop_front error timeout\n");
			break;
		}
	    ACE_OS::sleep(sleepTime);
	}
}

int main(int argc,char** argv)
{

	my_fifo<struct CapContent>* fifo=new my_berkeleyDBbased_fifo<struct CapContent>(1000);
	if (fifo->init()!=0)
	{
		delete fifo;
		system("pause");
		return 0;
	}
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)prod,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)prod,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)prod,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)cons,(void*)fifo);
	//ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)get_size,(void*)fifo);
	ACE_Thread_Manager::instance()->wait();

	delete fifo;


	//cap_http* ins=cap_http::get_instance();
	//ins->init();
	//ins->create_thread(5);
	//

	//TimeOut_Handler* th=new TimeOut_Handler(5);
	//TimeOut_Handler* th1=new TimeOut_Handler(6);
	//TimeOut_Handler* th2=new TimeOut_Handler(7);
	//th2->cancel_timer();
	//while(1)
	//{
 //       ACE_Reactor::instance()->handle_events();
	//}
	//cap_http::destroy_instance();
	system("pause");
	return 0;
};