#pragma once
#include <Windows.h>
#include <string>
using namespace std;
namespace core
{
	enum
	{
		CMD_MAX_CHAR	= 128,			// 最大输入字符数
		CMDCOUNT		= 50,			// 命令最大数量
		PARAM			= 10,			// 参数最大数量
		CMD_LEN			= 16,			// 每个参数的最大长度
		HELP_LEN		= 64,			// 帮助信息长度
		OUTPUT_BUFF_LEN	= 1024*4,		// 输出缓冲大小
	};

	class commandInput;

	typedef void (commandInput::* CMDFUN)();	//无参数命令函数
	typedef void (commandInput::* CMDFUN_PARAM)(int argc, char* argv[PARAM]);	//命令函数格式定义

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
		// 循环接收用户输入命令
		void run();

		// 自动向窗口输出消息
		void output_string(char* szFormat, ... );

		// 向窗口输出消息,解析命令时调用
		void output_line(char* szFormat, ... );

		void set_prompt(char* pstr );

	protected:

		// 添加命令到集合
		bool add_command(char* name, char* help, CMDFUN fun);
		bool add_command(char* name, char* help, CMDFUN_PARAM fun);
	
		// 解析命令
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


