#include "stdafx.h"
#include ".\pm_cmdinput.h"
#include "ffile.h"

ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = NULL;
ZWQUERYINFORMATIONFILE ZwQueryInformationFile=NULL;
DWORD Volumeserial[26];

BOOL LocateNtdllEntry ( void )
{
    BOOL    ret         = FALSE;
    char    NTDLL_DLL[] = "ntdll.dll";
    HMODULE ntdll_dll   = NULL;


    if ( ( ntdll_dll = GetModuleHandle( NTDLL_DLL ) ) == NULL )
    {
        printf("%s", "GetModuleHandle(\"ntdll.dll\") failed \r\n");
        return( FALSE );
    }

    if ( !( ZwQuerySystemInformation = ( ZWQUERYSYSTEMINFORMATION )GetProcAddress( ntdll_dll, "ZwQuerySystemInformation" ) ) )
    {
        printf( "GetProcAddress(ZwQuerySystemInformation) failed \r\n");
        ntdll_dll = NULL;
    }

    else if(!( ZwQueryInformationFile = ( ZWQUERYINFORMATIONFILE )GetProcAddress( ntdll_dll, "ZwQueryInformationFile" ) ) )
    {
        printf( "GetProcAddress(ZwQueryInformationFile) failed \r\n");
        ntdll_dll = NULL;
    }
    else ret = TRUE;

    return( ret );
}

void pm_cmdInput::getfindFile(int argc, char* argv[])
{

    //if(!LocateNtdllEntry())
    //{
    //    printf("≥ı ºªØ ß∞‹°£\n");
    //    return;
    //}


    //HANDLE hFile = ::CreateFileA("e:\\a.txt", 0, FILE_SHARE_WRITE,
    //    NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);




    //NTSTATUS status;
    //FILE_NAME_INFORMATION * psi = {0};
    //IO_STATUS_BLOCK ioStatus;
    //psi = (FILE_NAME_INFORMATION*)new WCHAR[sizeof(FILE_NAME_INFORMATION) + 1024];
    //memset(psi, 0, (sizeof(FILE_NAME_INFORMATION) + 1024)*2);

    //status = ZwQueryInformationFile(hFile, 
    //    &ioStatus, 
    //    psi, 
    //    sizeof(FILE_NAME_INFORMATION) + 1024 * sizeof(WCHAR), 
    //    FileNameInformation);



    //delete[] psi;
   
}
