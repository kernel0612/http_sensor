#include "proc_capCnt_block.h"


proc_capCnt_block::proc_capCnt_block(void):_totalBlockNum(0),_currBlockNum(1),_nextBlockNum(1)
{

}


proc_capCnt_block::~proc_capCnt_block(void)
{
}

int proc_capCnt_block::append_block_to_proccess(struct cap_content_block& block,struct CapContent& outCap)
{
	int min=0;
	my_ace_guard guard(_mutex);
	_totalBlockNum=block.TotalBlockNum;
	_currBlockNum=block.CurrBlockNum;
	if (block.CurrBlockNum==1)
	{
		_nextBlockNum=1;
	}
	if (_nextBlockNum==_currBlockNum)
	{

		_capBlocks.push_back(block);

		if (_currBlockNum==_totalBlockNum)
		{
			vector<struct cap_content_block>::const_iterator coit=_capBlocks.begin();
			if (coit->type==CLIENT_CNT)
			{
				memcpy(outCap.cliSrc,coit->Src,32);
				memcpy(outCap.cliDes,coit->Des,32);
				outCap.cliSport=coit->Sport;
				outCap.cliDport=coit->Dport;
				outCap.cliHasCnt=1;
				for (;coit!=_capBlocks.end();++coit)
				{
					min=((MAX_BUFF_SIZE-1)-outCap.cliCntSize)<coit->CntBlockSize?
						((MAX_BUFF_SIZE-1)-outCap.cliCntSize):coit->CntBlockSize;
					memcpy(outCap.cliCnt+outCap.cliCntSize,coit->CntBlock,min);
					outCap.cliCntSize+=min;
				}
			}
			else if (coit->type==SERVER_CNT)
			{
				memcpy(outCap.srvSrc,coit->Src,32);
				memcpy(outCap.srvDes,coit->Des,32);
				outCap.srvSport=coit->Sport;
				outCap.srvDport=coit->Dport;
				outCap.srvHasCnt=1;
				for (;coit!=_capBlocks.end();++coit)
				{
					min=((MAX_BUFF_SIZE-1)-outCap.srvCntSize)<coit->CntBlockSize?
						((MAX_BUFF_SIZE-1)-outCap.srvCntSize):coit->CntBlockSize;
					memcpy(outCap.srvCnt+outCap.srvCntSize,coit->CntBlock,min);
					outCap.srvCntSize+=min;
				}
			}
			_capBlocks.clear();
			return 0;
		}
	}
	_nextBlockNum=_currBlockNum+1;
	return -1;
}
