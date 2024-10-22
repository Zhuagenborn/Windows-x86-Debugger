/**
 * @file thread.h
 * @brief The thread.
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

#include "breakpoint.h"

#include <Windows.h>

#include <cstdint>
#include <functional>
#include <list>
#include <optional>

//! The step callback.
using StepCallback = std::function<void()>;

//! A thread.
class Thread {
public:
    /**
     * @brief Create a thread.
     *
     * @param handle The thread handle.
     * @param id The thread ID.
     * @param entry The entry address.
     * @param local_base The memory address of local data.
     */
    Thread(HANDLE handle, std::uint32_t id, std::uintptr_t entry,
           std::uintptr_t local_base) noexcept;

    Thread(Thread&& thread) noexcept;

    Thread(const Thread&) = delete;

    Thread& operator=(const Thread&) = delete;

    HANDLE Handle() const noexcept;

    std::uint32_t Id() const noexcept;

    std::uintptr_t Entry() const noexcept;

    std::uintptr_t LocalBase() const noexcept;

    //! Suspend the thread.
    void Suspend() const;

    //! Resume the thread.
    void Resume() const;

    //! Step into.
    void StepInto();

    //! Step into and set a single step callback.
    void StepInto(StepCallback callback);

    //! Whether the thread has set a single step.
    bool SingleStepping() const noexcept;

    //! Execute and clear single step callbacks.
    void ExecuteSingleStepCallbacks();

    //! Clear the single step.
    void ResetSingleStepping() noexcept;

    //! Perform an internal step.
    void InternalStep(StepCallback callback);

    //! Whether the thread has set an internal step.
    bool InternalStepping() const noexcept;

    //! Clear the internal step.
    void ResetInternalStepping() noexcept;

    //! Execute and clear the internal step callback.
    void ExecuteInternalStepCallback();

    /**
     * @brief Set a hardware breakpoint.
     *
     * @param address The memory address.
     * @param slot The hardware breakpoint slot.
     * @param type The hardware breakpoint type.
     * @param size The hardware breakpoint size.
     */
    void SetHardwareBreakpoint(std::uintptr_t address,
                               HardwareBreakpointSlot slot,
                               HardwareBreakpointType type,
                               HardwareBreakpointSize size) const;

    /**
     * @brief Delete a hardware breakpoint.
     *
     * @param slot The hardware breakpoint slot.
     */
    void DeleteHardwareBreakpoint(HardwareBreakpointSlot slot) const;

private:
    using StepCallbackList = std::list<StepCallback>;

    HANDLE handle_;

    std::uint32_t id_;

    std::uintptr_t entry_;

    std::uintptr_t local_base_;

    //! Whether the thread has set an internal step.
    bool internal_stepping_{ false };

    //! The internal step callback.
    StepCallback internal_step_callback_{};

    //! Whether the thread has set a single step.
    bool single_stepping_{ false };

    //! Single step callbacks.
    StepCallbackList single_step_callbacks_{};
};

//! An optional reference to a thread.
using OptionalThread = std::optional<std::reference_wrapper<Thread>>;