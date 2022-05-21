#include "process.h"

#include <algorithm>
#include <cassert>
#include <utility>


Process::Process(const HANDLE handle, const std::uint32_t id,
                 Thread&& main_thread,
                 const CREATE_PROCESS_DEBUG_INFO& create_info) noexcept :
    handle_{ handle },
    id_{ id },
    main_thread_id_{ main_thread.Id() },
    create_info_{ create_info } {
    NewThread(std::move(main_thread));
}

Process::Process(Process&& process) noexcept :
    handle_{ process.handle_ },
    id_{ process.id_ },
    main_thread_id_{ process.main_thread_id_ },
    create_info_{ process.create_info_ },
    hit_system_breakpoint_{ process.hit_system_breakpoint_ },
    threads_{ std::move(process.threads_) },
    debugged_thread_{ std::move(debugged_thread_) },
    breakpoint_callbacks_{ std::move(breakpoint_callbacks_) },
    software_breakpoints_{ std::move(software_breakpoints_) },
    hardware_breakpoints_{ std::move(hardware_breakpoints_) },
    hardware_breakpoint_slots_{ std::move(hardware_breakpoint_slots_) } {
    process.handle_ = nullptr;
    process.id_ = 0;
}

HANDLE Process::Handle() const noexcept {
    return handle_;
}

std::uint32_t Process::Id() const noexcept {
    return id_;
}

std::uint32_t Process::MainThreadId() const noexcept {
    return main_thread_id_;
}

CREATE_PROCESS_DEBUG_INFO Process::CreateInfo() const noexcept {
    return create_info_;
}

void Process::Suspend() const {
    std::ranges::for_each(threads_, [](auto& pair) {
        auto& [_, thread] = pair;
        thread.Suspend();
    });
}

void Process::Resume() const {
    std::ranges::for_each(threads_, [](auto& pair) {
        auto& [_, thread] = pair;
        thread.Resume();
    });
}

OptionalThread Process::DebuggedThread() const noexcept {
    return debugged_thread_;
}

OptionalThread Process::SetDebuggedThread(const std::uint32_t id) noexcept {
    debugged_thread_ = FindThread(id);
    return debugged_thread_;
}

void Process::ResetDebuggedThread() noexcept {
    debugged_thread_ = std::nullopt;
}

bool Process::HasDebuggedThread() const noexcept {
    return debugged_thread_.has_value();
}

bool Process::HasHitSystemBreakpoint() const noexcept {
    return hit_system_breakpoint_;
}

void Process::HitSystemBreakpoint() noexcept {
    hit_system_breakpoint_ = true;
}

void Process::ExecuteBreakpointCallback(const BreakpointKey key) {
    const auto callback_found = breakpoint_callbacks_.find(key);
    if (callback_found != breakpoint_callbacks_.cend()) {
        const auto [type, address] = key;
        switch (type) {
            case BreakpointType::Software: {
                const auto breakpoint_found =
                    software_breakpoints_.find(address);
                if (breakpoint_found != software_breakpoints_.cend()) {
                    callback_found->second(breakpoint_found->second);
                }

                break;
            }
            case BreakpointType::Hardware: {
                const auto breakpoint_found =
                    hardware_breakpoints_.find(address);
                if (breakpoint_found != hardware_breakpoints_.cend()) {
                    callback_found->second(breakpoint_found->second);
                }

                break;
            }
            case BreakpointType::Memory: {
                break;
            }
            default: {
                assert(false);
            }
        }

        breakpoint_callbacks_.erase(callback_found);
    }
}