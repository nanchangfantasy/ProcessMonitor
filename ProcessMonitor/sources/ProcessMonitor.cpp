// ProcessMonitor.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "pm_cmdInput.h"

int _tmain(int argc, _TCHAR* argv[])
{
    pm_cmdInput theApp;
    theApp.set_prompt("pm_cmd:>");

    theApp.run();

	return 0;
}

