#pragma once
#include "cmdCommon.h"

class cmdAddFriends :
	public cmdCommon
{
public:
	cmdAddFriends(void);
	~cmdAddFriends(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
};
