#pragma once
#include "cmdCommon.h"

class cmdNumZero :
	public cmdCommon
{
public:
	cmdNumZero(void);
	~cmdNumZero(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
