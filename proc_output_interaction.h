#ifndef PROC_OUTPUT_INTERACTION_H
#define PROC_OUTPUT_INTERACTION_H
#include "outputdef.h"
#include "ace/Mem_Map.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "ace/Date_Time.h"
class proc_output_interaction
{
public:
	proc_output_interaction(void);
	virtual ~proc_output_interaction(void);
	virtual int init();
	virtual int fini();
	virtual int process(struct outputSerInfo* srv , struct outputCliInfo* clt);

protected:
	int _flag;
};

#endif