#include "debugger.h"


void Debugger::OnCreateThread(const CREATE_THREAD_DEBUG_INFO& details) {
    DebuggedProcess().NewThread(
        { details.hThread, debug_event_.dwThreadId,
          reinterpret_cast<std::uintptr_t>(details.lpStartAddress),
          reinterpret_cast<std::uintptr_t>(details.lpThreadLocalBase) });

    SetDebuggedProcessThread(0, debug_event_.dwThreadId);

    cbCreateThread(details, DebuggedThread());
}

void Debugger::OnExitThread(const EXIT_THREAD_DEBUG_INFO& details) {
    cbExitThread(details, DebuggedThread());

    DebuggedProcess().RemoveThread(debug_event_.dwThreadId);

    ResetDebuggedProcessThread();
}