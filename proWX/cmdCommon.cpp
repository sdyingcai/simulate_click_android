#include "StdAfx.h"
#include "cmdCommon.h"

cmdCommon::cmdCommon(void)
{
}

cmdCommon::~cmdCommon(void)
{
}

void cmdCommon::init()
{
}

bool cmdCommon::openFile(_TCHAR* pFileName)
{
	bool bRet = false;
	CStdioFile cmdFile;
	if(cmdFile.Open(pFileName, CFile::modeRead | CFile::typeText))
	{
		TRACE("cmdFile.Open\r\n");
		while(1)
		{
			TCHAR buf[256] = {0};
			if(NULL == cmdFile.ReadString(buf, 255))
			{
				TRACE("NULL == cmdFile.ReadString\r\n");	
				break;
			}

			char bufCmd[512] = {0};
			WideCharToMultiByte( CP_ACP, 0, buf, -1,
				bufCmd, 512, NULL, NULL );

			bRet = parseFile(bufCmd);
		}

		cmdFile.Close();
	}
	else
	{
		TRACE("cmdFile.Open failure\r\n");
	}

	return bRet;
}

bool cmdCommon::parseFile(const char* pData)
{
	return false;
}

void cmdCommon::runListCmd()
{
	int nCount = mListCmd.size();
	int nCur = 0;

	list<string>::iterator itBegin = mListCmd.begin();
	for(; itBegin != mListCmd.end(); itBegin++)
	{
		runCmd(*itBegin);
		nCur++;
		if(14 == nCount && 7 == nCur)
		{
			Sleep(800);
		}
	}
}

int cmdCommon::covert16(char* pData)
{
	char *str;     
	return (int)strtol(pData, &str, 16);//Ê®Áù½øÖÆ 
}

void cmdCommon::runCmd(string& strCmd)
{
	TransAndroid trans;
	trans.sendEventContent("79817D8656DEEF00D02DD12B967AB30", strCmd.c_str());
}

void cmdCommon::runSleep(char* bufCmd)
{
	char columnOne[256] = {0};
	char columnTwo[256] = {0};

	sscanf(bufCmd, "%s %s", columnOne, columnTwo);

	long nValue = atol(columnTwo);
	Sleep(nValue);
}