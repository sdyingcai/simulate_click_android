#pragma once
#include "cmdcommon.h"

class cmdNumSeven :
	public cmdCommon
{
public:
	cmdNumSeven(void);
	~cmdNumSeven(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
