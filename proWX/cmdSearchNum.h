#pragma once
#include "cmdCommon.h"

class cmdSearchNum :
	public cmdCommon
{
public:
	cmdSearchNum(void);
	~cmdSearchNum(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
