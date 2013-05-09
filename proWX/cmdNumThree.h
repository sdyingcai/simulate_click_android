#pragma once
#include "cmdcommon.h"

class cmdNumThree :
	public cmdCommon
{
public:
	cmdNumThree(void);
	~cmdNumThree(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
