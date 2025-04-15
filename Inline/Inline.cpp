#include <windows.h>
#include <stdio.h>
int (WINAPI * pMessageBoxA)(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);

int  Hookem(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType)
{
    printf("Hello");
    return IDOK;

}

void Hooking()
{char patch[14] = {0}; 

pMessageBoxA =(int (WINAPI * )(HWND,LPCSTR,LPCSTR,UINT))GetProcAddress(GetModuleHandleA("user32.dll"),"MessageBoxA"); 
BYTE * Hooking = (BYTE *)&Hookem;

memcpy(patch,"\xFF\x25",2);
memcpy(patch+6,&Hooking,8);


WriteProcessMemory(GetCurrentProcess(),(LPVOID)pMessageBoxA,patch,sizeof(patch),NULL);
printf("Hookem address:%p",Hookem);





}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{ case DLL_PROCESS_ATTACH:
            Hooking();
    case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}