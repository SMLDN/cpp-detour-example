#include <windows.h>
#include "detours.h"


typedef unsigned int(__thiscall* _CCore_reload)(void* t, char* funcName);
_CCore_reload fxcore_reload = (_CCore_reload)0x10037750;

//_CCore_reload fxcore_reload = (_CCore_reload)0x10037750;


//class CMember
//{
//public:
//    unsigned int Target(char* funcName);
//};
//
//unsigned int CMember::Target(char* funcName)
//{
//    printf("  CMember::Target!   (this:%p)\n", this);
//}
//
//class CCore {
//public:
//    unsigned int reload(char* funcName);
//    static void (CCore::* Real_Target)(char* funcName);
//};
//
//unsigned int CCore::reload(char* funcName)
//{
//    (this->*Real_Target)(funcName);
//}



//debug
//std::ofstream ofs("zdn_hook.txt", std::ofstream::out | std::ofstream::app);

bool Match(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++bMask) {
        if (*szMask == 'x' && *pData != *bMask) {
            return false;
        }
    }
    return (*szMask) == NULL;
}


DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask)
{
    for (DWORD i = 0; i < dwLen; i++)
    {
        if (Match((BYTE*)(dwAddress + i), bMask, szMask)) {
            return (DWORD)(dwAddress + i);
        }
    }

    return 0;
}


unsigned int __fastcall zdn_reload(void *t, void* uu, char* funcName)
{
    ofs << "funcName: " << funcName << std::endl;
    return fxcore_reload(t, funcName);
}

BOOL APIENTRY DllMain(
    HANDLE hModule,
    DWORD reason,
    LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
              
        // A process is loading the DLL.
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(LPVOID&)fxcore_lua_gettop, (PBYTE)zdn_lua_gettop);

       //unsigned int (CMember:: * pfTarget)(char* funcName) = &(void*) 0x10037750;
       //unsigned int (CCore:: * pfMine)(char* funcName) = &CCore::reload;
       //
       //DetourAttach(&(PVOID&)CCore::Real_Target,
       //    *(PBYTE*)&pfMine);
        DetourAttach(&(LPVOID&)fxcore_reload, (PBYTE)zdn_reload);


        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}