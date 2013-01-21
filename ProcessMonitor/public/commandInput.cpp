#include "StdAfx.h"

#include "commandInput.h"
#include <iostream>

using namespace core;

commandInput::commandInput(void)
{
	memset(m_szPrompt, 0x0, sizeof(m_szPrompt) );
	strcpy(m_szPrompt, "server:");
	m_bCmdCount			= 0;
	m_bCmdParamCount	= 0;
	m_bRunning			= false;

	for(int i=0; i<PARAM; ++i )
	{
		m_argv[i] = (char*)malloc(HELP_LEN);   // �����죬����һ��Ҫ��;
	}

    add_command("cls",  "������",	        &commandInput::OnCls );
	add_command("quit", "�˳�����",		&commandInput::OnQuit );
	add_command("help", "��ʾ������Ϣ��",	    &commandInput::OnHelp );
}

commandInput::~commandInput(void)
{
	for(int i=0; i<PARAM; ++i )
	{
		 free(m_argv[i] );
		 m_argv[i] = NULL;
	}
}

void commandInput::run()
{
	char	szBuff[CMD_MAX_CHAR];
	m_bRunning = true;
	while(m_bRunning )
	{
		output_prompt();
		memset(szBuff, 0x0, CMD_MAX_CHAR);
		cin.getline(szBuff, CMD_MAX_CHAR );
		parseCommand(szBuff );
	}
}

void commandInput::parseCommand(char* szCmd )
{
	char	seps[]		= " \t,";
	BYTE	bLen		= sizeof(seps);
	USHORT	len			= (USHORT)strlen(szCmd);
	USHORT	i			= 0;
	USHORT	j			= 0;
	USHORT	paramNum	= 0;
	bool	bFind		= false;
	char*	pChar		= szCmd;

	if (len == 0 )
	{
		 return;
	}

    // ����ϴ�����;
    for(i=0; i<PARAM; ++i )
    {
        memset(m_argv[i], 0x0, HELP_LEN );
    }

	paramNum = 0;
	for(i=0; i<len; ++i)
	{
		if (	pChar[i] == seps[0] ||  
				pChar[i] == seps[1] || 
				pChar[i] == seps[2] )
		{
			j		= 0;
			bFind	= true;
			continue;
		}
		else
		{
			if (bFind == true )
			{
				++paramNum;
			}
			bFind = false;
			m_argv[paramNum][j++] = pChar[i];
		}
	}

	paramNum++;
	cmd_info* pCmdInfo = NULL;
	if (paramNum == 1 )
	{
		pCmdInfo = getCmdFun(pChar);
		if(pCmdInfo )
		{
			(this->*(pCmdInfo->cmdFun.cmd_fun))();
		}
		else
		{
			OnCommand();
		}
	}
	else
	{
		if (_stricmp("cmd", m_argv[0]) == 0 )
		{
			if (_stricmp("/c", m_argv[1]) != 0 )
			{
				output_line("cmd����ڶ�������Ӧ��Ϊ /c��");
				return;
			}
			system(pChar );
			return;
		}
		pCmdInfo = getCmdParamFun(m_argv[0]);
		if(pCmdInfo )
		{
			(this->*(pCmdInfo->cmdFun.cmd_param_fun))(paramNum, m_argv);
		}
		else
		{
			OnCommand();
		}
	}
}

void commandInput::output_string(char* szFormat, ... )
{
	cout<<endl;
	memset(m_szBuff, 0x0, sizeof(m_szBuff));

	va_list args;
	va_start(args, szFormat);
	vsprintf(m_szBuff, szFormat, args);
	va_end(args);
	cout<<m_szBuff<<endl;
	output_prompt();
}

void commandInput::output_line(char* szFormat, ... )
{
	memset(m_szBuff, 0x0, sizeof(m_szBuff));

	va_list args;
	va_start(args, szFormat);
	vsprintf(m_szBuff, szFormat, args);
	va_end(args);
	cout<<m_szBuff<<endl;
}

void commandInput::output_prompt()
{
	cout<< m_szPrompt;
}

bool commandInput::add_command(char* name, char* help, CMDFUN fun)
{
	if (m_bCmdCount >= CMDCOUNT )
	{
		 return false;
	}
	strcpy(m_cmdFun[m_bCmdCount].name, name );
	strcpy(m_cmdFun[m_bCmdCount].help, help );
	m_cmdFun[m_bCmdCount].cmdFun.cmd_fun = fun;
	++m_bCmdCount;
	return true;
}

bool commandInput::add_command(char* name, char* help, CMDFUN_PARAM fun)
{
	if (m_bCmdParamCount >= CMDCOUNT )
	{
		return false;
	}
	strcpy(m_cmdParamFun[m_bCmdParamCount].name, name );
	strcpy(m_cmdParamFun[m_bCmdParamCount].help, help );
	m_cmdParamFun[m_bCmdParamCount].cmdFun.cmd_param_fun = fun;
	++m_bCmdParamCount;
	return true;
}		


cmd_info* commandInput::getCmdFun(char* pName )
{
	for (BYTE i=0; i< m_bCmdCount; ++i )
	{
		if (_stricmp(pName, m_cmdFun[i].name) == 0 )
		{
			 return &m_cmdFun[i];
		}
	}
	return NULL;
}

cmd_info* commandInput::getCmdParamFun(char* pName )
{
	for (BYTE i=0; i< m_bCmdParamCount; ++i )
	{
		if (_stricmp(pName, m_cmdParamFun[i].name) == 0 )
		{
			return &m_cmdParamFun[i];
		}
	}
	return NULL;
}

void commandInput::OnQuit()
{
	m_bRunning = false;
}

void commandInput::OnHelp()
{
	BYTE i = 0;
	cout<<"�޲������б�"<<endl;
	for(i = 0; i< m_bCmdCount; ++i )
	{
		cout<<m_cmdFun[i].name<<"\t\t"<<m_cmdFun[i].help<<endl;
	}
	cout<<"�в������б�"<<endl;
	cout<<"cmd\t\t����ϵͳ���"<<endl;
	for(i = 0; i< m_bCmdParamCount; ++i )
	{
		cout<<m_cmdParamFun[i].name<<"\t\t"<<m_cmdParamFun[i].help<<endl;
	}
}

void commandInput::OnCls()
{
    system("cmd /c cls");
}

void commandInput::OnCommand()
{
	cout<<"δ����������!"<<endl;
}

void commandInput::set_prompt(char* pstr )
{
	if (pstr )
	{
		strcpy(m_szPrompt, pstr);
	}
}
