#pragma once
#include "cmdCommon.h"

class cmdSearchFriends :
	public cmdCommon
{
public:
	cmdSearchFriends(void);
	~cmdSearchFriends(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
