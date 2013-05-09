#pragma once
#include "cmdCommon.h"
#include "cmdNumZero.h"
#include "cmdNumOne.h"
#include "cmdNumTwo.h"
#include "cmdNumThree.h"
#include "cmdNumFour.h"
#include "cmdNumFive.h"
#include "cmdNumSix.h"
#include "cmdNumSeven.h"
#include "cmdNumEight.h"
#include "cmdNumNine.h"

class cmdInputKetNum :
	public cmdCommon
{
public:
	cmdInputKetNum(void);
	~cmdInputKetNum(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
	virtual void runListCmd();
	void parseNum(int nNum);
	void setCmd(string strCmd);

public:
	string mStrCmd;
	list<int> mListIntCmd;

public:
	cmdNumZero numZero;
	cmdNumOne numOne;
	cmdNumTwo numTwo;
	cmdNumThree numThree;
	cmdNumFour numFour;
	cmdNumFive numFive;
	cmdNumSix numSix;
	cmdNumSeven numSeven;
	cmdNumEight numEight;
	cmdNumNine numNine;
};
