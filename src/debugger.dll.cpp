#include "debugger.h"


void Debugger::OnLoadDll(const LOAD_DLL_DEBUG_INFO& details) {
    cbLoadDll(details);

    if (details.hFile) {
        CloseHandle(details.hFile);
    }
}

void Debugger::OnUnloadDll(const UNLOAD_DLL_DEBUG_INFO& details) {
    cbUnloadDll(details);
}