#include "proc_capCnt_block.h"


proc_capCnt_block::proc_capCnt_block(void):_totalBlockNum(0),_currBlockNum(1),_nextBlockNum(1)
{

}


proc_capCnt_block::~proc_capCnt_block(void)
{
}

int proc_capCnt_block::append_block_to_proccess(struct cap_content_block& block,struct CapContent& outCap)
{
	my_ace_guard guard(_mutex);
	_totalBlockNum=block.totalBlockNum;
	_currBlockNum=block.currBlockNum;
	if (block.currBlockNum==1)
	{
		_nextBlockNum=1;
	}
	if (_nextBlockNum==_currBlockNum)
	{
		_capBlocks.push_back(block);
		if (_currBlockNum==_totalBlockNum)
		{
			//
			vector<struct cap_content_block>::const_iterator coit=_capBlocks.begin();
			memcpy(outCap.srvSrc,coit->srvSrc,32);
			memcpy(outCap.srvDes,coit->srvDes,32);
			outCap.srvSport=coit->srvSport;
			outCap.srvDport=coit->srvDport;
			outCap.srvHasCnt=coit->srvHasCnt;
			memcpy(outCap.cliSrc,coit->cliSrc,32);
			memcpy(outCap.cliDes,coit->cliDes,32);
			outCap.cliSport=coit->cliSport;
			outCap.cliDport=coit->cliDport;
			outCap.cliHasCnt=coit->cliHasCnt;
			for (;coit!=_capBlocks.end();++coit)
			{
				memcpy(outCap.srvCnt+outCap.srvCntSize,coit->srvCntBlock,coit->srvCntBlockSize);
				outCap.srvCntSize+=coit->srvCntBlockSize;
				memcpy(outCap.cliCnt+outCap.cliCntSize,coit->cliCntBlock,coit->cliCntBlockSize);
				outCap.cliCntSize+=coit->cliCntBlockSize;
			}

			_capBlocks.clear();
			return 0;
		}
	}
	_nextBlockNum=_currBlockNum+1;
	return -1;
}
