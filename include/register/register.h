/**
 * @file register.h
 * @brief The register controller.
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

#include <cstdint>


enum class RegisterIndex {
    EAX,
    EBX,
    ECX,
    EDX,
    ESP,
    EBP,
    ESI,
    EDI,
    EIP,
    DR0,
    DR1,
    DR2,
    DR3,
    DR6,
    DR7,
    EFLAGS
};

class Registers;

/**
 * A basic register controller.
 * It's an interface for controlling a x86 register.
 */
class Register {
public:
    Register(Registers& registers, RegisterIndex index) noexcept;

    virtual ~Register() noexcept = default;

    Register& operator=(std::uintptr_t value) noexcept;

    Register& operator++() noexcept;

    Register operator++(int) noexcept;

    Register& operator--() noexcept;

    Register operator--(int) noexcept;

    Register& operator+=(std::uintptr_t value) noexcept;

    Register& operator-=(std::uintptr_t value) noexcept;

    /**
     * Set the register value.
     */
    void Set(std::uintptr_t value) noexcept;

    /**
     * Reset the register value to zero.
     */
    void Reset() noexcept;

    /**
     * Get the register value.
     */
    std::uintptr_t Get() const noexcept;

protected:
    RegisterIndex index_;

    Registers& registers_;
};

bool operator==(const Register& register1, const Register& register2) noexcept;

bool operator!=(const Register& register1, const Register& register2) noexcept;

enum class Flag { CF, PF, AF, ZF, SF, TF, IF, DF, OF };

/**
 * A FLAGS register controller, providing interfaces to set flags.
 */
class FlagRegister : public Register {
public:
    FlagRegister(Registers& registers) noexcept;

    void SetCF() noexcept;
    void SetPF() noexcept;
    void SetAF() noexcept;
    void SetZF() noexcept;
    void SetSF() noexcept;
    void SetTF() noexcept;
    void SetIF() noexcept;
    void SetDF() noexcept;
    void SetOF() noexcept;

    void ResetCF() noexcept;
    void ResetPF() noexcept;
    void ResetAF() noexcept;
    void ResetZF() noexcept;
    void ResetSF() noexcept;
    void ResetTF() noexcept;
    void ResetIF() noexcept;
    void ResetDF() noexcept;
    void ResetOF() noexcept;

    bool CF() const noexcept;
    bool PF() const noexcept;
    bool AF() const noexcept;
    bool ZF() const noexcept;
    bool SF() const noexcept;
    bool TF() const noexcept;
    bool IF() const noexcept;
    bool DF() const noexcept;
    bool OF() const noexcept;

    void Set(Flag flag) noexcept;

    void Reset(Flag flag) noexcept;

    bool Get(Flag flag) const noexcept;

private:
    struct Flags {
        std::uintptr_t CF : 1;
        std::uintptr_t reserve0 : 1;
        std::uintptr_t PF : 1;
        std::uintptr_t reserve1 : 1;
        std::uintptr_t AF : 1;
        std::uintptr_t reserve2 : 1;
        std::uintptr_t ZF : 1;
        std::uintptr_t SF : 1;
        std::uintptr_t TF : 1;
        std::uintptr_t IF : 1;
        std::uintptr_t DF : 1;
        std::uintptr_t OF : 1;
        std::uintptr_t reserve3 : 20;
    };

    void SetCF(bool set) noexcept;
    void SetPF(bool set) noexcept;
    void SetAF(bool set) noexcept;
    void SetZF(bool set) noexcept;
    void SetSF(bool set) noexcept;
    void SetTF(bool set) noexcept;
    void SetIF(bool set) noexcept;
    void SetDF(bool set) noexcept;
    void SetOF(bool set) noexcept;

    void Set(Flag flag, bool set) noexcept;
};

/**
 * A debug status register @p DR6 controller, providing interfaces to set debug status.
 */
class DebugStatusRegister : public Register {
public:
    DebugStatusRegister(Registers& registers) noexcept;

    void SetB0() noexcept;
    void SetB1() noexcept;
    void SetB2() noexcept;
    void SetB3() noexcept;

    void ResetB0() noexcept;
    void ResetB1() noexcept;
    void ResetB2() noexcept;
    void ResetB3() noexcept;

    bool B0() const noexcept;
    bool B1() const noexcept;
    bool B2() const noexcept;
    bool B3() const noexcept;

private:
    struct DR6 {
        std::uintptr_t B0 : 1;
        std::uintptr_t B1 : 1;
        std::uintptr_t B2 : 1;
        std::uintptr_t B3 : 1;
        std::uintptr_t reserve0 : 9;
        std::uintptr_t BD : 1;
        std::uintptr_t BS : 1;
        std::uintptr_t BT : 1;
        std::uintptr_t reserve1 : 16;
    };

    void SetB0(bool set) noexcept;
    void SetB1(bool set) noexcept;
    void SetB2(bool set) noexcept;
    void SetB3(bool set) noexcept;
};

/**
 * A debug control register @p DR7 controller, providing interfaces to control debugging.
 */
class DebugControlRegister : public Register {
public:
    DebugControlRegister(Registers& registers) noexcept;

    void SetL0() noexcept;
    void SetL1() noexcept;
    void SetL2() noexcept;
    void SetL3() noexcept;

    void ResetL0() noexcept;
    void ResetL1() noexcept;
    void ResetL2() noexcept;
    void ResetL3() noexcept;

    bool L0() const noexcept;
    bool L1() const noexcept;
    bool L2() const noexcept;
    bool L3() const noexcept;

    void SetRW0(std::uintptr_t value) noexcept;
    void SetRW1(std::uintptr_t value) noexcept;
    void SetRW2(std::uintptr_t value) noexcept;
    void SetRW3(std::uintptr_t value) noexcept;

    std::uintptr_t RW0() const noexcept;
    std::uintptr_t RW1() const noexcept;
    std::uintptr_t RW2() const noexcept;
    std::uintptr_t RW3() const noexcept;

    void SetLEN0(std::uintptr_t value) noexcept;
    void SetLEN1(std::uintptr_t value) noexcept;
    void SetLEN2(std::uintptr_t value) noexcept;
    void SetLEN3(std::uintptr_t value) noexcept;

    std::uintptr_t LEN0() const noexcept;
    std::uintptr_t LEN1() const noexcept;
    std::uintptr_t LEN2() const noexcept;
    std::uintptr_t LEN3() const noexcept;

private:
    struct DR7 {
        std::uintptr_t L0 : 1;
        std::uintptr_t G0 : 1;
        std::uintptr_t L1 : 1;
        std::uintptr_t G1 : 1;
        std::uintptr_t L2 : 1;
        std::uintptr_t G2 : 1;
        std::uintptr_t L3 : 1;
        std::uintptr_t G3 : 1;
        std::uintptr_t LE : 1;
        std::uintptr_t GE : 1;
        std::uintptr_t reserve0 : 3;
        std::uintptr_t GD : 1;
        std::uintptr_t reserve1 : 2;
        std::uintptr_t RW0 : 2;
        std::uintptr_t LEN0 : 2;
        std::uintptr_t RW1 : 2;
        std::uintptr_t LEN1 : 2;
        std::uintptr_t RW2 : 2;
        std::uintptr_t LEN2 : 2;
        std::uintptr_t RW3 : 2;
        std::uintptr_t LEN3 : 2;
    };

    void SetL0(bool set) noexcept;
    void SetL1(bool set) noexcept;
    void SetL2(bool set) noexcept;
    void SetL3(bool set) noexcept;
};