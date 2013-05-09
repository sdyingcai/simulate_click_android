#pragma once
#include "cmdCommon.h"

class cmdAuthOK :
	public cmdCommon
{
public:
	cmdAuthOK(void);
	~cmdAuthOK(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
