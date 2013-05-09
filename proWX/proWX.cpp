// proWX.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cmdFactory.h"

int _tmain(int argc, _TCHAR* argv[])
{
	cmdFactory fact;
	fact.init();
	fact.runListCmd();
	return 0;
}

