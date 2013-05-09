#pragma once
#include "cmdCommon.h"

class cmdNumSix :
	public cmdCommon
{
public:
	cmdNumSix(void);
	~cmdNumSix(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
