/**
 * @file registers.h
 * @brief x86 32-bit registers.
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

#include "register.h"

#include <Windows.h>

#include <cstdint>

/**
 * x86 32-bit registers.
 */
class Registers final {
public:
    Register EAX;
    Register EBX;
    Register ECX;
    Register EDX;

    Register EIP;

    Register ESP;
    Register EBP;

    Register ESI;
    Register EDI;

    FlagRegister EFLAGS;

    Register DR0;
    Register DR1;
    Register DR2;
    Register DR3;

    DebugStatusRegister DR6;
    DebugControlRegister DR7;

    /**
     * Get register values from a thread.
     */
    Registers(const HANDLE thread,
              const std::uint32_t context_flags = CONTEXT_ALL);

    /**
     * Set register values to the thread.
     */
    ~Registers();

    Registers(const Registers&) = delete;

    Registers& operator=(const Registers&) = delete;

    HANDLE Thread() const noexcept;

    void Set(const RegisterIndex index, const std::uintptr_t value) noexcept;

    void Reset(const RegisterIndex index) noexcept;

    std::uintptr_t Get(const RegisterIndex index) const noexcept;

private:
    const HANDLE thread_;

    //! Original values.
    const CONTEXT original_context_{};

    //! Modified values.
    CONTEXT context_{};
};