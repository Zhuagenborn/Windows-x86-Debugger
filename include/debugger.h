/**
 * @file debugger.h
 * @brief The basic debugger.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-11-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#pragma once

#include "process.h"
#include "thread.h"

#include <Windows.h>

#include <cstdint>
#include <exception>
#include <string_view>
#include <unordered_map>

/**
 * A basic debugger.
 */
class Debugger {
public:
    Debugger() noexcept = default;

    Debugger(const Debugger&) = delete;

    Debugger& operator=(const Debugger&) = delete;

    virtual ~Debugger() noexcept;

    /**
     * @brief Create a process to debug.
     *
     * @param file_path         The file path.
     * @param cmd_line          The command line.
     * @param current_directory The current directory.
     * @param start_suspended   Whether to suspend the process after creation.
     */
    void Create(std::wstring_view file_path, std::wstring_view cmd_line,
                std::wstring_view current_directory, bool start_suspended);

    /**
     * @brief Attach to a process to debug.
     *
     * @param process_id    The process ID.
     */
    void Attach(std::uint32_t process_id);

    /**
     * Start the debug loop.
     */
    void Start();

    /**
     * Detach the process.
     */
    void UnsafeDetach();

    /**
     * Detach the process.
     * The detach happens at the end of the debug loop.
     */
    void Detach();

    /**
     * Terminate the process.
     */
    void Stop();

protected:
    using ProcessMap = std::unordered_map<std::uint32_t, Process>;

    /*************** Debug event callbacks ***************/

    //! The callback for create-process events.
    virtual void OnCreateProcess(const CREATE_PROCESS_DEBUG_INFO& details);

    //! The callback for exit-process events.
    virtual void OnExitProcess(const EXIT_PROCESS_DEBUG_INFO& details);

    //! The callback for create-thread events.
    virtual void OnCreateThread(const CREATE_THREAD_DEBUG_INFO& details);

    //! The callback for exit-thread events.
    virtual void OnExitThread(const EXIT_THREAD_DEBUG_INFO& details);

    //! The callback for load-dynamic-link-library events.
    virtual void OnLoadDll(const LOAD_DLL_DEBUG_INFO& details);

    //! The callback for unload-dynamic-link-library events.
    virtual void OnUnloadDll(const UNLOAD_DLL_DEBUG_INFO& details);

    //! The callback for exception events.
    virtual void OnException(const EXCEPTION_DEBUG_INFO& details);

    //! The callback for output-debugging-string events.
    virtual void OnOutputString(const OUTPUT_DEBUG_STRING_INFO& details);

    //! The callback for RIP events.
    virtual void OnRip(const RIP_INFO& details);

    //! The callback for unknown events.
    virtual void OnUnknownEvent(std::uint32_t event_code);

    /**************** Exception callbacks ****************/

    //! The callback for single steps.
    virtual void OnSingleStep(const EXCEPTION_RECORD& record,
                              bool first_chance);

    //! The callback for breakpoint encounters.
    virtual void OnBreakpoint(const EXCEPTION_RECORD& record,
                              bool first_chance);

    //! The callback for memory access violations.
    virtual void OnAccessViolation(const EXCEPTION_RECORD& record,
                                   bool first_chance);

    //! The callback for hardware breakpoint encounters.
    virtual void OnHardwareBreakpoint(std::uintptr_t address);

    /*****************************************************/

    /**
     * Clear debug cache.
     */
    virtual void ClearCache() noexcept;

    /**
     * @brief Set the debugged process and thread.
     *
     * @param process_id    The process ID.
     * @param thread_id     The thread ID.
     */
    void SetDebuggedProcessThread(std::uint32_t process_id,
                                  std::uint32_t thread_id) noexcept;

    /**
     * Reset the debugged process and thread to null.
     */
    void ResetDebuggedProcessThread() noexcept;

    /**
     * Get the debugged process.
     */
    Process& DebuggedProcess() const noexcept;

    /**
     * Get the debugged thread.
     */
    Thread& DebuggedThread() const noexcept;

    /**
     * Whether a process is being debugged.
     */
    bool HasDebuggedProcess() const noexcept;

    /**
     * Whether a thread is being debugged.
     */
    bool HasDebuggedThread() const noexcept;

    /**
     * Create a process.
     */
    void NewProcess(Process&& process) noexcept;

    /**
     * @brief Remove a process.
     *
     * @param id    The process ID.
     */
    bool RemoveProcess(std::uint32_t id) noexcept;

    /**
     * @brief Find a process.
     *
     * @param id    The process ID.
     */
    OptionalProcess FindProcess(std::uint32_t id) const noexcept;


    //! Whether the debug loop is running.
    volatile bool debugging_{ false };

    //! Whether the debugger has detached the process.
    volatile bool detached_{ false };

    //! Whether the debugger has attached to a process.
    volatile bool attached_{ false };

    //! Whether the main process has exited.
    volatile bool main_process_exited_{ false };

    STARTUPINFOW main_startup_{};

    PROCESS_INFORMATION main_process_{};

    DEBUG_EVENT debug_event_{};

    std::uint32_t continue_status_{ DBG_EXCEPTION_NOT_HANDLED };

    //! The processes created by the main process.
    ProcessMap processes_{};

    //! The debugged process.
    OptionalProcess debugged_process_{};

    //! The debugged thread.
    OptionalThread debugged_thread_{};

private:
    /****************** Other callbacks ******************/

    //! The callback for internal errors in the debug loop.
    virtual void cbInternalLoopError(const std::exception& error){};

    //! The generic callback for debug events, called before they are internally processed.
    virtual void cbPreDebugEvent(const DEBUG_EVENT& event){};

    //! The generic callback for debug events, called after they were internally processed.
    virtual void cbPostDebugEvent(const DEBUG_EVENT& event){};

    //! The generic callback for unknown debug events, called before they are internally processed.
    virtual void cbUnknownEvent(std::uint32_t event_code){};

    //! The generic callback for exceptions, called before they are internally processed.
    virtual void cbPreException(const EXCEPTION_RECORD& record,
                                bool first_chance){};

    //! The callback for create-process events, called after they were internally processed.
    virtual void cbCreateProcess(const CREATE_PROCESS_DEBUG_INFO& details,
                                 const Process& process){};

    //! The callback for attach-process events.
    virtual void cbAttachProcess(const CREATE_PROCESS_DEBUG_INFO& details,
                                 const Process& process){};

    //! The callback for exit-process events, called before they are internally processed.
    virtual void cbExitProcess(const EXIT_PROCESS_DEBUG_INFO& details,
                               const Process& process){};

    //! The callback for create-thread events, called after they were internally processed.
    virtual void cbCreateThread(const CREATE_THREAD_DEBUG_INFO& details,
                                const Thread& thread){};

    //! The callback for exit-thread events, called before they are internally processed.
    virtual void cbExitThread(const EXIT_THREAD_DEBUG_INFO& details,
                              const Thread& thread){};

    //! The callback for load-dynamic-link-library events, called after they were internally processed.
    virtual void cbLoadDll(const LOAD_DLL_DEBUG_INFO& details){};

    //! The callback for unload-dynamic-link-library events, called before they are internally processed.
    virtual void cbUnloadDll(const UNLOAD_DLL_DEBUG_INFO& details){};

    //! The callback for output-debugging-string events, called before they are internally processed.
    virtual void cbOutputString(const OUTPUT_DEBUG_STRING_INFO& details){};

    //! The callback for RIP events, called before they are internally processed.
    virtual void cbRip(const RIP_INFO& details){};

    //! The callback for system breakpoint encounters, called after they were internally processed.
    virtual void cbSystemBreakpoint(const Process& process){};

    //! The generic callback for breakpoint encounters, called before user callbacks.
    virtual void cbBreakpoint(const Breakpoint& breakpoint){};

    //! The callback for entry breakpoint encounters, called after the generic breakpoint callback.
    virtual void cbEntryBreakpoint(const Process& process){};

    //! The generic callback for single steps, called before user callbacks.
    virtual void cbStep(const Thread& thread){};

    //! The generic callback for unhandled exceptions.
    virtual void cbUnhandledException(const EXCEPTION_RECORD& record,
                                      bool first_chance){};
};