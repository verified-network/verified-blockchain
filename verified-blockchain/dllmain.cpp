#include "pch.h"
#include "node.h"
#include "peer.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{   
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            node n;
            n.start();
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        {
            node n;
            n.stop();            
            break;
        }
    }
    return TRUE;
}





