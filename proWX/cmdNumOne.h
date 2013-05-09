#pragma once
#include "cmdCommon.h"

class cmdNumOne :
	public cmdCommon
{
public:
	cmdNumOne(void);
	~cmdNumOne(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
