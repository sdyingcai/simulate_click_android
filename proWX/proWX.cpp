// proWX.cpp : �������̨Ӧ�ó������ڵ㡣
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

