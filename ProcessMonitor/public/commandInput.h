#pragma once
#include <Windows.h>
#include <string>
using namespace std;
namespace core
{
	enum
	{
		CMD_MAX_CHAR	= 128,			// ��������ַ���
		CMDCOUNT		= 50,			// �����������
		PARAM			= 10,			// �����������
		CMD_LEN			= 16,			// ÿ����������󳤶�
		HELP_LEN		= 64,			// ������Ϣ����
		OUTPUT_BUFF_LEN	= 1024*4,		// ��������С
	};

	class commandInput;

	typedef void (commandInput::* CMDFUN)();	//�޲��������
	typedef void (commandInput::* CMDFUN_PARAM)(int argc, char* argv[PARAM]);	//�������ʽ����

	struct cmd_info
	{
		CHAR	name[CMD_LEN];
		CHAR	help[HELP_LEN];
		union cmdfun
		{
			CMDFUN			cmd_fun;
			CMDFUN_PARAM	cmd_param_fun;
		}cmdFun;

		cmd_info()
		{
			memset(this, 0x0, sizeof(cmd_info));
		}
	};

	class commandInput
	{
	public:
		commandInput(void);
		virtual ~commandInput(void);

	public:
		// ѭ�������û���������
		void run();

		// �Զ��򴰿������Ϣ
		void output_string(char* szFormat, ... );

		// �򴰿������Ϣ,��������ʱ����
		void output_line(char* szFormat, ... );

		void set_prompt(char* pstr );

	protected:

		// ����������
		bool add_command(char* name, char* help, CMDFUN fun);
		bool add_command(char* name, char* help, CMDFUN_PARAM fun);
	
		// ��������
		void parseCommand(char* command );

		void OnCommand();

    protected:
		void output_prompt();

		void OnQuit();
		void OnHelp();
        void OnCls();

		cmd_info*	getCmdFun(char* pName );
		cmd_info*	getCmdParamFun(char* pName );

	protected:
		CHAR			m_szPrompt[64];
		cmd_info		m_cmdFun[CMDCOUNT];
		cmd_info		m_cmdParamFun[CMDCOUNT];
		BYTE			m_bCmdCount;
		BYTE			m_bCmdParamCount;
		bool			m_bRunning;
		CHAR*			m_argv[PARAM];
		CHAR			m_szBuff[OUTPUT_BUFF_LEN];
	};
}


