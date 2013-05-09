#pragma once
#include "cmdCommon.h"

class cmdNumNine :
	public cmdCommon
{
public:
	cmdNumNine(void);
	~cmdNumNine(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
