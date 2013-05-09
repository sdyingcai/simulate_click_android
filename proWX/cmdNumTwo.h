#pragma once
#include "cmdCommon.h"

class cmdNumTwo :
	public cmdCommon
{
public:
	cmdNumTwo(void);
	~cmdNumTwo(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
