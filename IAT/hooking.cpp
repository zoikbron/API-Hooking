#include <windows.h>
#include <stdio.h>
#include <DbgHelp.h>
#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"dbghelp.lib")

int (WINAPI * pMessageBoxA)(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType) = MessageBoxA;

int Hooked(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType){
        printf("Hello My boys");
        pMessageBoxA(NULL,"eo","eo",MB_OK);
return IDOK;

}


void Hooking(char * dll,char * OrigFunc){
        ULONG size;
        DWORD oldprotect;
        HANDLE Proc = GetModuleHandleA(NULL);

        IMAGE_IMPORT_DESCRIPTOR * pImportDirectory = (IMAGE_IMPORT_DESCRIPTOR *) ImageDirectoryEntryToDataEx(Proc,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&size,NULL);
        
        ULONGLONG ProcAddress =(ULONGLONG)GetProcAddress(GetModuleHandleA(dll),OrigFunc);



        for(int i =0; pImportDirectory[i].Name != 0;i++){
                char * sTemp = (char *)((BYTE *)Proc + pImportDirectory[i].Name);
        if(lstrcmpiA(sTemp,dll)==0){
            IMAGE_THUNK_DATA * thunk = (IMAGE_THUNK_DATA *)((BYTE *)Proc + pImportDirectory[i].FirstThunk);
            for(int n = 0; thunk[n].u1.Function !=0;n++){

                if(thunk[n].u1.Function == ProcAddress ){
                        VirtualProtect(&thunk[n].u1.Function,4096,PAGE_READWRITE,&oldprotect);
                        thunk[n].u1.Function = (ULONGLONG)&Hooked;
                        VirtualProtect(&thunk[n].u1.Function,4096,oldprotect,&oldprotect);


                }

            }
        }

        }


    

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
            Hooking("user32.dll","MessageBoxA");
    case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
