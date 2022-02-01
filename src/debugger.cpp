#include "debugger.h"
#include "error.h"
#include "register/registers.h"

#include <cassert>
#include <cwchar>
#include <memory>


Debugger::~Debugger() noexcept {
    ClearCache();
}

void Debugger::Create(const std::wstring_view file_path,
                      const std::wstring_view cmd_line,
                      const std::wstring_view current_directory,
                      const bool start_suspended) {
    ClearCache();

    const auto raw_cmd_line =
        std::make_unique<wchar_t[]>(cmd_line.length() + 1);
    wcsncpy_s(raw_cmd_line.get(), cmd_line.length() + 1, cmd_line.data(),
              cmd_line.length());

    if (!CreateProcessW(file_path.data(), raw_cmd_line.get(), nullptr, nullptr,
                        false,
                        DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE
                            | (start_suspended ? CREATE_SUSPENDED : 0),
                        nullptr, current_directory.data(), &main_startup_,
                        &main_process_)) {
        ThrowLastError();
    }
}

void Debugger::Attach(const std::uint32_t process_id) {
    ClearCache();

    if (!DebugActiveProcess(process_id)) {
        ThrowLastError();
    }

    attached_ = true;
}

void Debugger::Start() {
    debugging_ = true;

    while (!main_process_exited_) {
        try {
            if (!WaitForDebugEvent(&debug_event_, INFINITE)) {
                ThrowLastError();
            }

            continue_status_ = DBG_EXCEPTION_NOT_HANDLED;

            SetDebuggedProcessThread(debug_event_.dwProcessId,
                                     debug_event_.dwThreadId);

            cbPreDebugEvent(debug_event_);

            switch (debug_event_.dwDebugEventCode) {
                case CREATE_PROCESS_DEBUG_EVENT: {
                    OnCreateProcess(debug_event_.u.CreateProcessInfo);
                    break;
                }
                case EXIT_PROCESS_DEBUG_EVENT: {
                    OnExitProcess(debug_event_.u.ExitProcess);
                    break;
                }
                case CREATE_THREAD_DEBUG_EVENT: {
                    OnCreateThread(debug_event_.u.CreateThread);
                    break;
                }
                case EXIT_THREAD_DEBUG_EVENT: {
                    OnExitThread(debug_event_.u.ExitThread);
                    break;
                }
                case LOAD_DLL_DEBUG_EVENT: {
                    OnLoadDll(debug_event_.u.LoadDll);
                    break;
                }
                case UNLOAD_DLL_DEBUG_EVENT: {
                    OnUnloadDll(debug_event_.u.UnloadDll);
                    break;
                }
                case EXCEPTION_DEBUG_EVENT: {
                    OnException(debug_event_.u.Exception);
                    break;
                }
                case OUTPUT_DEBUG_STRING_EVENT: {
                    OnOutputString(debug_event_.u.DebugString);
                    break;
                }
                case RIP_EVENT: {
                    OnRip(debug_event_.u.RipInfo);
                    break;
                }
                default: {
                    OnUnknownEvent(debug_event_.dwDebugEventCode);
                    break;
                }
            }

            cbPostDebugEvent(debug_event_);

            if (HasDebuggedThread()) {
                Registers{ DebuggedThread().Handle(), CONTEXT_DEBUG_REGISTERS }
                    .DR6.Reset();
            }

            if (!ContinueDebugEvent(debug_event_.dwProcessId,
                                    debug_event_.dwThreadId,
                                    continue_status_)) {
                break;
            }

            if (detached_) {
                UnsafeDetach();
                break;
            }

        } catch (const std::exception& error) {
            cbInternalLoopError(error);
        }
    }

    debugging_ = false;
}

void Debugger::UnsafeDetach() {
    if (HasDebuggedThread()) {
        Registers(DebuggedThread().Handle(), CONTEXT_CONTROL).EFLAGS.ResetTF();
    }

    if (!DebugActiveProcessStop(main_process_.dwProcessId)) {
        ThrowLastError();
    }
}

void Debugger::Detach() {
    detached_ = true;
}

void Debugger::Stop() {
    if (!TerminateProcess(main_process_.hProcess, EXIT_SUCCESS)) {
        ThrowLastError();
    }
}

void Debugger::ClearCache() noexcept {
    ResetDebuggedProcessThread();

    if (main_process_.hThread) {
        CloseHandle(main_process_.hThread);
    }

    if (main_process_.hProcess) {
        CloseHandle(main_process_.hProcess);
    }

    main_process_ = {};
    main_startup_ = {};
    debug_event_ = {};
    processes_.clear();
    attached_ = false;
    detached_ = false;
    main_process_exited_ = false;
    debugging_ = false;
    continue_status_ = DBG_EXCEPTION_NOT_HANDLED;
}

Process& Debugger::DebuggedProcess() const noexcept {
    assert(HasDebuggedProcess());
    return debugged_process_->get();
}

Thread& Debugger::DebuggedThread() const noexcept {
    assert(HasDebuggedThread());
    return debugged_thread_->get();
}

bool Debugger::HasDebuggedProcess() const noexcept {
    return debugged_process_.has_value();
}

bool Debugger::HasDebuggedThread() const noexcept {
    return debugged_thread_.has_value();
}

void Debugger::ResetDebuggedProcessThread() noexcept {
    debugged_thread_ = std::nullopt;
    if (HasDebuggedProcess()) {
        DebuggedProcess().ResetDebuggedThread();
        debugged_process_ = std::nullopt;
    }
}

void Debugger::SetDebuggedProcessThread(
    const std::uint32_t process_id, const std::uint32_t thread_id) noexcept {
    if (process_id != 0) {
        debugged_process_ = FindProcess(process_id);
    }

    if (debugged_process_) {
        debugged_thread_ =
            debugged_process_->get().SetDebuggedThread(thread_id);
    }
}