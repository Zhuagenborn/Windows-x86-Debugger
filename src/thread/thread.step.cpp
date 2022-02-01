#include "thread.h"
#include "register/registers.h"

#include <algorithm>
#include <utility>


void Thread::StepInto() {
    Registers{ handle_ }.EFLAGS.SetTF();
    single_stepping_ = true;
}

void Thread::StepInto(StepCallback callback) {
    StepInto();
    single_step_callbacks_.push_back(std::move(callback));
}

void Thread::InternalStep(StepCallback callback) {
    Registers{ handle_ }.EFLAGS.SetTF();
    internal_step_callback_ = std::move(callback);
    internal_stepping_ = true;
}

bool Thread::SingleStepping() const noexcept {
    return single_stepping_;
}

void Thread::ResetSingleStepping() noexcept {
    single_stepping_ = false;
}

bool Thread::InternalStepping() const noexcept {
    return internal_stepping_;
}

void Thread::ResetInternalStepping() noexcept {
    internal_stepping_ = false;
}

void Thread::ExecuteInternalStepCallback() {
    if (internal_step_callback_) {
        internal_step_callback_();
        internal_step_callback_ = nullptr;
    }
}

void Thread::ExecuteSingleStepCallbacks() {
    std::ranges::for_each(single_step_callbacks_,
                          [](auto& callback) { callback(); });
    single_step_callbacks_.clear();
}