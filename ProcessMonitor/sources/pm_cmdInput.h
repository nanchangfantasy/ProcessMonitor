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

    // ��ȡָ������ID���ӽ����б�;
    void getSubProcesslist(int argc, char* argv[]);

    // ��ȡָ������ID�ĸ�������Ϣ;
    void getProcessInfo(int argc, char* argv[]);

    // ��ȡ�����б�;
    void getProcessList();

    // ��ȡָ��ģ����Ϣ;
    void findModuleInfo(int argc, char* argv[]);

    // ��ȡ���̵�ģ��;
    void getProcessModulelist(int argc, char* argv[]);

    // �鿴�ļ������ĸ��������򿪡�
    void getfindFile(int argc, char* argv[]);

};
