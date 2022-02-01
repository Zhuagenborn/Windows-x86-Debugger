#include "thread.h"
#include "error.h"

#include <utility>


Thread::Thread(const HANDLE handle, const std::uint32_t id,
               const std::uintptr_t entry,
               const std::uintptr_t local_base) noexcept :
    handle_{ handle }, id_{ id }, entry_{ entry }, local_base_{ local_base } {}

Thread::Thread(Thread&& thread) noexcept :
    handle_{ thread.handle_ },
    id_{ thread.id_ },
    entry_{ thread.entry_ },
    local_base_{ thread.local_base_ },
    single_step_callbacks_{ std::move(thread.single_step_callbacks_) },
    single_stepping_{ thread.single_stepping_ },
    internal_stepping_{ thread.internal_stepping_ },
    internal_step_callback_{ std::move(thread.internal_step_callback_) } {
    const_cast<HANDLE&>(thread.handle_) = nullptr;
    const_cast<std::uint32_t&>(thread.id_) = 0;
}

HANDLE Thread::Handle() const noexcept {
    return handle_;
}

std::uintptr_t Thread::Entry() const noexcept {
    return entry_;
}

std::uintptr_t Thread::LocalBase() const noexcept {
    return local_base_;
}

std::uint32_t Thread::Id() const noexcept {
    return id_;
}

void Thread::Suspend() const {
    if (SuspendThread(handle_) == -1) {
        ThrowLastError();
    }
}

void Thread::Resume() const {
    if (ResumeThread(handle_) == -1) {
        ThrowLastError();
    }
}