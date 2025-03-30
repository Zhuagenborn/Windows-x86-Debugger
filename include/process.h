/**
 * @file process.h
 * @brief The process.
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
#include "thread.h"

#include <Windows.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

//! A process.
class Process {
public:
    /**
     * @brief Create a process.
     *
     * @param handle The process handle.
     * @param id The process ID.
     * @param main_thread The main thread.
     * @param create_info The creation information.
     */
    Process(HANDLE handle, std::uint32_t id, Thread&& main_thread,
            const CREATE_PROCESS_DEBUG_INFO& create_info) noexcept;

    Process(Process&& process) noexcept;

    Process(const Process&) = delete;

    Process& operator=(const Process&) = delete;

    HANDLE Handle() const noexcept;

    std::uint32_t Id() const noexcept;

    std::uint32_t MainThreadId() const noexcept;

    CREATE_PROCESS_DEBUG_INFO CreateInfo() const noexcept;

    //! Suspend the process.
    void Suspend() const;

    //! Resume the process.
    void Resume() const;

    /**
     * @brief Find a thread.
     *
     * @param id The thread ID.
     */
    OptionalThread FindThread(std::uint32_t id) const noexcept;

    //! Create a thread.
    void NewThread(Thread&& thread) noexcept;

    /**
     * @brief Remove a thread.
     *
     * @param id The thread ID.
     */
    bool RemoveThread(std::uint32_t id) noexcept;

    //! Get the debugged thread.
    OptionalThread DebuggedThread() const noexcept;

    /**
     * @brief Set the debugged thread and return it.
     *
     * @param id The thread ID.
     */
    OptionalThread SetDebuggedThread(std::uint32_t id) noexcept;

    //! Reset the debugged thread to null.
    void ResetDebuggedThread() noexcept;

    //! Whether a thread is being debugged.
    bool HasDebuggedThread() const noexcept;

    //! Whether the process has hit the system breakpoint.
    bool HasHitSystemBreakpoint() const noexcept;

    //! Hit the system breakpoint.
    void HitSystemBreakpoint() noexcept;

    //! Whether a memory address is valid.
    bool ValidMemory(std::uintptr_t address) const noexcept;

    /**
     * @brief Write data to a memory area.
     *
     * @param address The memory address.
     * @param data The data.
     * @param safe Safe or unsafe writting.
     * @return Original memory data.
     */
    std::vector<std::byte> WriteMemory(std::uintptr_t address,
                                       std::span<const std::byte> data,
                                       bool safe) const;

    /**
     * @brief Safely write data to a memory area, filtering out breakpoint bytes.
     *
     * @param address The memory address.
     * @param data The data.
     * @return Original memory data.
     */
    std::vector<std::byte> WriteMemorySafe(
        std::uintptr_t address, std::span<const std::byte> data) const;

    /**
     * @brief Unsafely write data to a memory area.
     *
     * @param address The memory address.
     * @param data The data.
     * @return Original memory data.
     */
    std::vector<std::byte> WriteMemoryUnsafe(
        std::uintptr_t address, std::span<const std::byte> data) const;

    /**
     * @brief Read data from a memory area.
     *
     * @param address The memory address.
     * @param size The size to read.
     * @param safe Safe or unsafe reading.
     * @return Memory data.
     */
    std::vector<std::byte> ReadMemory(std::uintptr_t address, std::size_t size,
                                      bool safe) const;

    /**
     * @brief Safely read data from a memory area, filtering out breakpoint bytes.
     *
     * @param address The memory address.
     * @param size The size to read.
     * @return Memory data.
     */
    std::vector<std::byte> ReadMemorySafe(std::uintptr_t address,
                                          std::size_t size) const;

    /**
     * @brief Unsafely read data from a memory area.
     *
     * @param address The memory address.
     * @param size The size to read.
     * @return Memory data.
     */
    std::vector<std::byte> ReadMemoryUnsafe(std::uintptr_t address,
                                            std::size_t size) const;

    /**
     * @brief Find a free hardware breakpoint slot.
     *
     * @param[out] slot A free hardware breakpoint slot.
     * @return @p true if there is a free hardware breakpoint slot, otherwise @p false.
     */
    bool FindFreeHardwareBreakpointSlot(
        HardwareBreakpointSlot& slot) const noexcept;

    /**
     * @brief Set a hardware breakpoint.
     *
     * @param address The memory address.
     * @param slot The hardware breakpoint slot.
     * @param type The hardware breakpoint type.
     * @param size The hardware breakpoint size.
     * @param single_shoot Whether to set a one-time breakpoint.
     * @param callback A callback function.
     */
    void SetHardwareBreakpoint(std::uintptr_t address,
                               HardwareBreakpointSlot slot,
                               HardwareBreakpointType type,
                               HardwareBreakpointSize size,
                               bool single_shoot = false,
                               BreakpointCallback callback = {});

    /**
     * @brief Delete a hardware breakpoint.
     *
     * @param address The memory address.
     * @return @p true if it succeeds, otherwise @p false.
     */
    bool DeleteHardwareBreakpoint(std::uintptr_t address);

    /**
     * @brief Find a hardware breakpoint.
     *
     * @param address The memory address.
     */
    std::optional<HardwareBreakpoint> FindHardwareBreakpoint(
        std::uintptr_t address) const noexcept;

    /**
     * @brief Set a software breakpoint.
     *
     * @param address The memory address.
     * @param single_shoot Whether to set a one-time breakpoint.
     * @param callback A callback function.
     */
    void SetSoftwareBreakpoint(std::uintptr_t address,
                               bool single_shoot = false,
                               BreakpointCallback callback = {});

    /**
     * @brief Delete a software breakpoint.
     *
     * @param address The memory address.
     * @return @p true if it succeeds, otherwise @p false.
     */
    bool DeleteSoftwareBreakpoint(std::uintptr_t address);

    /**
     * @brief Find a software breakpoint.
     *
     * @param address The memory address.
     */
    std::optional<SoftwareBreakpoint> FindSoftwareBreakpoint(
        std::uintptr_t address) const noexcept;

    /**
     * @brief Set `INT3` instruction.
     *
     * @param address The memory address.
     * @param[out] original_byte The original byte.
     */
    void SetInt3(std::uintptr_t address,
                 std::byte* original_byte = nullptr) const;

    /**
     * @brief Delete `INT3` instruction.
     *
     * @param address The memory address.
     * @param original_byte The original byte.
     */
    void DeleteInt3(std::uintptr_t address, std::byte original_byte) const;

    /**
     * @brief Execute and clear a breakpoint's callback.
     *
     * @param breakpoint The breakpoint key.
     */
    void ExecuteBreakpointCallback(BreakpointKey breakpoint);

private:
    using ThreadMap = std::unordered_map<std::uint32_t, Thread>;

    template <ValidBreakpoint BP>
    using BreakpointMap = std::map<std::uintptr_t, BP>;

    using HardwareBreakpointSlots =
        std::map<HardwareBreakpointSlot, HardwareBreakpoint*>;

    HANDLE handle_;

    std::uint32_t id_;

    std::uint32_t main_thread_id_;

    CREATE_PROCESS_DEBUG_INFO create_info_;

    //! Whether the process has hit the system breakpoint.
    bool hit_system_breakpoint_{ false };

    //! The threads created by the process.
    ThreadMap threads_{};

    //! The debugged thread.
    OptionalThread debugged_thread_{};

    BreakpointMap<SoftwareBreakpoint> software_breakpoints_{};

    BreakpointMap<HardwareBreakpoint> hardware_breakpoints_{};

    HardwareBreakpointSlots hardware_breakpoint_slots_{};

    std::map<BreakpointKey, BreakpointCallback> breakpoint_callbacks_{};
};

//! An optional reference to a process.
using OptionalProcess = std::optional<std::reference_wrapper<Process>>;