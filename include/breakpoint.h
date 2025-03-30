/**
 * @file breakpoint.h
 * @brief Breakpoints.
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

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>


enum class BreakpointType { Software, Hardware, Memory };

//! Basic breakpoint data.
struct Breakpoint {
    Breakpoint(std::uintptr_t address, BreakpointType type,
               bool single_shoot) noexcept;

    virtual ~Breakpoint() noexcept = default;

    std::uintptr_t address;

    BreakpointType type;

    //! Whether this is a one-time breakpoint.
    bool single_shoot{ false };
};


//! The number of hardware breakpoint slots, from @p DR0 to @p DR3.
inline constexpr std::size_t hardware_breakpoint_slot_count{ 4 };

enum class HardwareBreakpointSlot { DR0 = 0, DR1 = 1, DR2 = 2, DR3 = 3 };

enum class HardwareBreakpointType : std::uint32_t {
    Execute = 0B00,
    Write = 0B01,
    ReadWrite = 0B11
};

enum class HardwareBreakpointSize : std::uint32_t {
    Byte = 0B00,
    Word = 0B01,
    Dword = 0B11
};

struct HardwareBreakpoint : public Breakpoint {
    HardwareBreakpoint(std::uintptr_t address, HardwareBreakpointSlot slot,
                       HardwareBreakpointType access,
                       HardwareBreakpointSize size, bool single_shoot) noexcept;

    HardwareBreakpointSlot slot;

    HardwareBreakpointType access;

    HardwareBreakpointSize size;
};


//! The machine code for `INT3` instruction.
inline constexpr std::byte int_3{ 0xCC };

struct SoftwareBreakpoint : public Breakpoint {
    SoftwareBreakpoint(std::uintptr_t address, std::byte original_byte,
                       bool single_shoot) noexcept;

    std::byte original_byte;
};


template <typename T>
concept ValidBreakpoint = std::derived_from<T, Breakpoint>;

//! The breakpoint callback.
using BreakpointCallback = std::function<void(const Breakpoint&)>;

//! A key to uniquely identify a breakpoint.
using BreakpointKey = std::pair<BreakpointType, std::uintptr_t>;