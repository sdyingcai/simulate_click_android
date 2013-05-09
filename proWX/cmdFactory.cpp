#include "StdAfx.h"
#include "cmdFactory.h"

cmdFactory::cmdFactory(void)
: cmdCommon()
{
}

cmdFactory::~cmdFactory(void)
{
}

void cmdFactory::init()
{
	searchNum.init();
	setKeyNum.init();
	inputKeyNum.init();
	searchFriends.init();
	addFriends.init();
	authOK.init();
	backKey.init();
	openFile(L"d:/yingcai/project/proWX/Debug/cmd/InputKetNum.txt");
}

bool cmdFactory::parseFile(const char* bufCmd)
{
	//char columnOne[256] = {0};
	//char columnTwo[256] = {0};
	//char columnThree[256] = {0};
	//char columnFour[256] = {0};
	//string strCmd = "shell:sendevent ";
	//sscanf(bufCmd, "%s %s %s %s", columnOne, columnTwo, columnThree, columnFour);
	//int nLen = strlen(columnOne);
	//columnOne[nLen-1] = 0;
	//strCmd.append(columnOne);
	//strCmd.append(" ");

	//char arrTwo[256] = {0};
	//itoa(covert16(columnTwo), arrTwo, 10);
	//strCmd.append(arrTwo);
	//strCmd.append(" ");

	//char arrThree[256] = {0};
	//itoa(covert16(columnThree), arrThree, 10);
	//strCmd.append(arrThree);
	//strCmd.append(" ");

	//char arrFour[256] = {0};
	//itoa(covert16(columnFour), arrFour, 10);
	//strCmd.append(arrFour);

	mListCmd.push_back(bufCmd);

	return true;
}

void cmdFactory::runListCmd()
{
	list<string>::iterator itBegin = mListCmd.begin();
	for(; itBegin != mListCmd.end(); itBegin++)
	{
		inputKeyNum.setCmd(*itBegin);

		searchNum.runListCmd();
		Sleep(1000);

		setKeyNum.runListCmd();
		//Sleep(1000);
		
		inputKeyNum.runListCmd();

		searchFriends.runListCmd();
		Sleep(2000);

		addFriends.runListCmd();
		Sleep(1000);

		authOK.runListCmd();
		Sleep(5000);

		backKey.runListCmd();
		Sleep(2000);

		backKey.runListCmd();
	}
}