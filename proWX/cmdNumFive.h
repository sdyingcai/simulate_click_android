#pragma once
#include "cmdCommon.h"

class cmdNumFive :
	public cmdCommon
{
public:
	cmdNumFive(void);
	~cmdNumFive(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
