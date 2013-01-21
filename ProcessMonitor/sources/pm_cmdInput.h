#pragma once

#include "commandInput.h"

using namespace core;


class pm_cmdInput :
    public commandInput
{
public:
    pm_cmdInput(void);
    ~pm_cmdInput(void);

private:

    // 获取指定进程ID的子进程列表;
    void getSubProcesslist(int argc, char* argv[]);

    // 获取指定进程ID的父进程信息;
    void getProcessInfo(int argc, char* argv[]);

    // 获取进程列表;
    void getProcessList();

    // 获取指定模块信息;
    void findModuleInfo(int argc, char* argv[]);

    // 获取进程的模块;
    void getProcessModulelist(int argc, char* argv[]);

    // 查看文件名被哪个进程所打开。
    void getfindFile(int argc, char* argv[]);

};
