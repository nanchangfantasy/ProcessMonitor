#include "StdAfx.h"
#include ".\pm_cmdinput.h"
#include <Tlhelp32.h>

#include <iostream>
using namespace std;


static void printProcessModuleList(DWORD dwPid_)
{
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid_ );
    if(hModuleSnap == INVALID_HANDLE_VALUE )  
    {
        return;
    }

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32 );

    if(!Module32First(hModuleSnap, &me32 ) ) 
    {
        CloseHandle(hModuleSnap );
    }

    DWORD _dwIndex = 1;
    do 
    {
        printf("[%08X -- %08X] %s\n", (DWORD)(_W64)me32.modBaseAddr, (DWORD)(_W64)me32.modBaseAddr + (DWORD)me32.modBaseSize, me32.szExePath);
        _dwIndex++;

    } while (Module32Next(hModuleSnap, &me32));

    printf("[%d] number of modules.\n", _dwIndex);

    ::CloseHandle(hModuleSnap);
}

pm_cmdInput::pm_cmdInput(void)
{
    add_command("plist",   "get process list.", (CMDFUN)&pm_cmdInput::getProcessList );
    add_command("subp",    "subp pid *get the sub process list of the special pid.", (CMDFUN_PARAM)&pm_cmdInput::getSubProcesslist );
    add_command("pinfo",   "pinfo pid *get the special process info.", (CMDFUN_PARAM)&pm_cmdInput::getProcessInfo );
    add_command("fmod",    "fmod modulename *在系统中查询指定的模块。", (CMDFUN_PARAM)&pm_cmdInput::findModuleInfo );
    add_command("pmod",    "pmod pid *枚举进程的模块。", (CMDFUN_PARAM)&pm_cmdInput::getProcessModulelist);
    add_command("ffile",   "ffile name *查看文件被谁打开。", (CMDFUN_PARAM)&pm_cmdInput::getfindFile);
}

pm_cmdInput::~pm_cmdInput(void)
{
}

void pm_cmdInput::getSubProcesslist(int argc, char* argv[])
{
    DWORD _dwTheID = atoi(argv[1]);
    if(!_dwTheID)   return;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, _dwTheID );
    if(hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32 );

    if(!Process32First(hProcessSnap, &pe32 ) )
    {
        CloseHandle(hProcessSnap );
        return;
    }

    DWORD _dwIndex = 1;
    do
    {
        if(pe32.th32ParentProcessID == _dwTheID)
        {
            std::cout<<"--sub process--------"<<std::endl;
            std::cout<<"index:\t"<<_dwIndex<<std::endl;
            std::cout<<"pid:\t"<<pe32.th32ProcessID<<std::endl;
            std::cout<<"pname:\t"<<pe32.szExeFile<<std::endl;
            _dwIndex++;
        }

    }while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap );
}

void pm_cmdInput::getProcessInfo(int argc, char* argv[])
{
    DWORD _dwTheID = atoi(argv[1]);
    if(!_dwTheID)   return;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, _dwTheID );
    if(hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32 );

    if(!Process32First(hProcessSnap, &pe32 ) )
    {
        CloseHandle(hProcessSnap );
        return;
    }

    do
    {
        if(pe32.th32ProcessID == _dwTheID)
        {
            std::cout<<"--process info--------"<<std::endl;
            std::cout<<"pid:\t"<<pe32.th32ProcessID<<std::endl;
            std::cout<<"ppid:\t"<<pe32.th32ParentProcessID<<std::endl;
            std::cout<<"pname:\t"<<pe32.szExeFile<<std::endl;
            break;
        }

    }while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap );
}

void pm_cmdInput::getProcessList()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 );
    if(hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32 );

    if(!Process32First(hProcessSnap, &pe32 ) )
    {
        CloseHandle(hProcessSnap );
        return;
    }

    printf("PID\tPPID\tIMAGE\n");
    printf("-------------------------------------\n");

    do
    {
        printf("%d\t%d\t%s\n", pe32.th32ProcessID, pe32.th32ParentProcessID, pe32.szExeFile);

    }while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap );
}

void pm_cmdInput::findModuleInfo(int argc, char* argv[])
{
    char* pModuleName = argv[1];
    
    if( !pModuleName || !strlen(pModuleName)) return;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 );
    if(hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32 );

    if(!Process32First(hProcessSnap, &pe32 ) )
    {
        CloseHandle(hProcessSnap );
        return;
    }
    
    DWORD _dwTotalSize = 0;

    do
    {
        HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID );
        if(hModuleSnap == INVALID_HANDLE_VALUE )  
        {
            continue;
        }

        MODULEENTRY32 me32;
        me32.dwSize = sizeof(MODULEENTRY32 );

        if(!Module32First(hModuleSnap, &me32 ) ) 
        {
            CloseHandle(hModuleSnap );
        }

        do 
        {
            if(_strcmpi(me32.szModule, pModuleName) == 0 )
            {
                printf("---------\n");
                printf("pid:\t%d\n", pe32.th32ProcessID);
                printf("pname:\t%s\n", pe32.szExeFile);
                printf("mname:\t%s\n", me32.szExePath);
                _dwTotalSize++;
            }

        } while (Module32Next(hModuleSnap, &me32));
        ::CloseHandle(hModuleSnap);

    }while(Process32Next(hProcessSnap, &pe32));

    printf("[%d] number of times loaded.\n", _dwTotalSize);

    CloseHandle(hProcessSnap );

}

void pm_cmdInput::getProcessModulelist(int argc, char* argv[])
{
    DWORD _dwTheID = atoi(argv[1]);
    if(!_dwTheID)   return;
    printProcessModuleList(_dwTheID);
}

