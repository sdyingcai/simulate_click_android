#pragma once
#include "cmdCommon.h"

class cmdNumFour :
	public cmdCommon
{
public:
	cmdNumFour(void);
	~cmdNumFour(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
