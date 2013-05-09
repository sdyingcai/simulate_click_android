#pragma once
#include "cmdCommon.h"

class cmdSetKeyNum :
	public cmdCommon
{
public:
	cmdSetKeyNum(void);
	~cmdSetKeyNum(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
