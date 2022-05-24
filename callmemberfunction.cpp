#include <windows.h>

class Target {
public:
    typedef void(__fastcall *_target_func)(void* pt, void* uuu, const char* text);
    _target_func target_func = (_load)0x007f1451; // member function pointer
};

void doMain() {
    void* uua;
    Target* zP = reinterpret_cast<Target*>(0x0074FE48);// this pointer
    zP->load = (Target::_load)0x007f1451;
    (*zP).load(zP, &uua, "inject by zdn");
    //auto fp = &Target::load;
    //(*zP.*fp)("injecte by zdn");
}

bool APIENTRY DllMain(
    HANDLE hModule,	 
    DWORD reason,
    LPVOID lpReserved) {

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        // A process is loading the DLL.
        doMain();
        break;
    case DLL_THREAD_ATTACH:
        // A process is creating a new thread.
        break;
    case DLL_THREAD_DETACH:
        // A thread exits normally.
        break;
    case DLL_PROCESS_DETACH:
        // A process unloads the DLL.
        break;
    }

    return true;
}