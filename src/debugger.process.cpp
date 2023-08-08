#include "debugger.h"

#include <utility>


void Debugger::OnCreateProcess(const CREATE_PROCESS_DEBUG_INFO& details) {
    bool attached{ false };
    if (attached_ && !main_process_.hProcess) {
        main_process_.hProcess = details.hProcess;
        main_process_.hThread = details.hThread;
        main_process_.dwProcessId = debug_event_.dwProcessId;
        main_process_.dwThreadId = debug_event_.dwThreadId;
        attached = true;
    }

    Thread thread{ details.hThread, debug_event_.dwThreadId,
                   reinterpret_cast<std::uintptr_t>(details.lpStartAddress),
                   reinterpret_cast<std::uintptr_t>(
                       details.lpThreadLocalBase) };

    Process process{ details.hProcess, debug_event_.dwProcessId,
                     std::move(thread), details };

    NewProcess(std::move(process));

    SetDebuggedProcessThread(debug_event_.dwProcessId, debug_event_.dwThreadId);

    cbCreateProcess(details, DebuggedProcess());

    if (attached) {
        cbAttachProcess(details, DebuggedProcess());
    } else {
        DebuggedProcess().SetSoftwareBreakpoint(DebuggedThread().Entry(), true);
    }

    if (details.hFile) {
        CloseHandle(details.hFile);
    }
}

void Debugger::OnExitProcess(const EXIT_PROCESS_DEBUG_INFO& details) {
    if (debug_event_.dwProcessId == main_process_.dwProcessId) {
        main_process_exited_ = true;
    }

    cbExitProcess(details, DebuggedProcess());

    RemoveProcess(debug_event_.dwProcessId);

    ResetDebuggedProcessThread();
}

void Debugger::NewProcess(Process&& process) noexcept {
    processes_.insert({ process.Id(), std::move(process) });
}

bool Debugger::RemoveProcess(const std::uint32_t id) noexcept {
    return processes_.erase(id) != 0;
}

OptionalProcess Debugger::FindProcess(const std::uint32_t id) const noexcept {
    const auto found{ processes_.find(id) };
    return found != processes_.cend()
               ? OptionalProcess{ const_cast<Process&>(found->second) }
               : std::nullopt;
}