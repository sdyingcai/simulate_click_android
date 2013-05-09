#pragma once

#include "TransAndroid.h"
#include <list>

class cmdCommon
{
public:
	cmdCommon(void);
	~cmdCommon(void);

public:
	virtual void init();
	virtual bool openFile(_TCHAR* pFileName);
	virtual bool parseFile(const char* pData);
	virtual int covert16(char* pData);
	virtual void runListCmd();
	virtual void runCmd(string& strCmd);
	virtual void runSleep(char* bufCmd);

public:
	list<string> mListCmd;
};
