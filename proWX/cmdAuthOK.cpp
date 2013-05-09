#include "StdAfx.h"
#include "cmdAuthOK.h"

cmdAuthOK::cmdAuthOK(void)
: cmdCommon()
{
}

cmdAuthOK::~cmdAuthOK(void)
{
}

void cmdAuthOK::init()
{
	openFile(L"d:/yingcai/project/proWX/Debug/cmd/AuthOK.txt");
}

bool cmdAuthOK::parseFile(const char* bufCmd)
{
	char columnOne[256] = {0};
	char columnTwo[256] = {0};
	char columnThree[256] = {0};
	char columnFour[256] = {0};
	string strCmd = "shell:sendevent ";
	sscanf(bufCmd, "%s %s %s %s", columnOne, columnTwo, columnThree, columnFour);
	int nLen = strlen(columnOne);
	columnOne[nLen-1] = 0;
	strCmd.append(columnOne);
	strCmd.append(" ");

	char arrTwo[256] = {0};
	itoa(covert16(columnTwo), arrTwo, 10);
	strCmd.append(arrTwo);
	strCmd.append(" ");

	char arrThree[256] = {0};
	itoa(covert16(columnThree), arrThree, 10);
	strCmd.append(arrThree);
	strCmd.append(" ");

	char arrFour[256] = {0};
	itoa(covert16(columnFour), arrFour, 10);
	strCmd.append(arrFour);

	mListCmd.push_back(strCmd);

	return true;
}



