#pragma once
#include "cmdCommon.h"

class cmdBackKey :
	public cmdCommon
{
public:
	cmdBackKey(void);
	~cmdBackKey(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
