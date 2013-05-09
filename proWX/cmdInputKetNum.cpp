#include "StdAfx.h"
#include "cmdInputKetNum.h"

cmdInputKetNum::cmdInputKetNum(void)
: cmdCommon()
{
}

cmdInputKetNum::~cmdInputKetNum(void)
{
}

void cmdInputKetNum::init()
{
	numZero.init();
	numOne.init();
	numTwo.init();
	numThree.init();
	numFour.init();
	numFive.init();
	numSix.init();
	numSeven.init();
	numEight.init();
	numNine.init();
	//openFile(L"cmd/InputKetNum.txt");
}

void cmdInputKetNum::setCmd(string strCmd)
{
	mStrCmd = strCmd;
}

bool cmdInputKetNum::parseFile(const char* bufCmd)
{
	string strBuf;
	strBuf.append(bufCmd);
	int nFind = strBuf.find('\r');
	if(string::npos != nFind)
	{
		strBuf.erase(nFind, 1);
	}

	nFind = strBuf.find('\n');
	if(string::npos != nFind)
	{
		strBuf.erase(nFind, 1);
	}

	int nValue = atoi(strBuf.c_str());
	int nLen = strBuf.size();
	for(int n = 1; n < nLen + 1; n++)
	{
		int nNum = nValue%10;
		nValue = nValue/10;
		mListIntCmd.push_front(nNum);
	}

	return true;
}

void cmdInputKetNum::runListCmd()
{
	parseFile(mStrCmd.c_str());
	list<int>::iterator itBegin = mListIntCmd.begin();
	for(; itBegin != mListIntCmd.end(); itBegin++)
	{
		parseNum(*itBegin);
	}
}

void cmdInputKetNum::parseNum(int nNum)
{
	switch(nNum)
	{
	case 0:
		{
			numZero.runListCmd();
		}
		break;
	case 1:
		{
			numOne.runListCmd();
		}
		break;
	case 2:
		{
			numTwo.runListCmd();
		}
		break;
	case 3:
		{
			numThree.runListCmd();
		}
		break;
	case 4:
		{
			numFour.runListCmd();
		}
		break;
	case 5:
		{
			numFive.runListCmd();
		}
		break;
	case 6:
		{
			numSix.runListCmd();
		}
		break;
	case 7:
		{
			numSeven.runListCmd();
		}
		break;
	case 8:
		{
			numEight.runListCmd();
		}
		break;
	case 9:
		{
			numNine.runListCmd();
		}
		break;
	}
}
