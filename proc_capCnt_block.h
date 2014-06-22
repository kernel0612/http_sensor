#ifndef PROC_CAPCNT_BLOCK_H
#define PROC_CAPCNT_BLOCK_H
#include "commondef.h"
#include "my_ace_guard.h"
#include <vector>
#include <iostream>
using namespace std;
class proc_capCnt_block
{
public:
	proc_capCnt_block(void);
	~proc_capCnt_block(void);

     int append_block_to_proccess(struct cap_content_block& block,struct CapContent& outCap);
private:
	ACE_Thread_Mutex _mutex;
	unsigned short _currBlockNum;
	unsigned short _totalBlockNum;
	unsigned short _nextBlockNum;
	vector<struct cap_content_block> _capBlocks;
};

#endif