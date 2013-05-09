#pragma once

#include "cmdSearchNum.h"
#include "cmdSetKeyNum.h"
#include "cmdInputKetNum.h"
#include "cmdSearchFriends.h"
#include "cmdSearchFriends.h"
#include "cmdAddFriends.h"
#include "cmdAuthOK.h"
#include "cmdBackKey.h"

class cmdFactory :
	public cmdCommon
{
public:
	cmdFactory(void);
	~cmdFactory(void);

public:
	virtual void init();
	virtual bool parseFile(const char* pData);
	virtual void runListCmd();

private:
	cmdSearchNum searchNum;
	cmdSetKeyNum setKeyNum;
	cmdInputKetNum inputKeyNum;
	cmdSearchFriends searchFriends;
	cmdAddFriends addFriends;
	cmdAuthOK authOK;
	cmdBackKey backKey;
};
