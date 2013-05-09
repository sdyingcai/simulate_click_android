#pragma once
#include "cmdCommon.h"

class cmdNumEight :
	public cmdCommon
{
public:
	cmdNumEight(void);
	~cmdNumEight(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
