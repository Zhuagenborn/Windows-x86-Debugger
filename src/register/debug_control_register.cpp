#include "register.h"
#include "registers.h"

#include <cassert>


DebugControlRegister::DebugControlRegister(Registers& registers) noexcept :
    Register{ registers, RegisterIndex::DR7 } {}

void DebugControlRegister::SetL0() noexcept {
    SetL0(true);
}

void DebugControlRegister::SetL1() noexcept {
    SetL1(true);
}

void DebugControlRegister::SetL2() noexcept {
    SetL2(true);
}

void DebugControlRegister::SetL3() noexcept {
    SetL3(true);
}

void DebugControlRegister::ResetL0() noexcept {
    SetL0(false);
}

void DebugControlRegister::ResetL1() noexcept {
    SetL1(false);
}

void DebugControlRegister::ResetL2() noexcept {
    SetL2(false);
}

void DebugControlRegister::ResetL3() noexcept {
    SetL3(false);
}


bool DebugControlRegister::L0() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).L0;
}

bool DebugControlRegister::L1() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).L1;
}

bool DebugControlRegister::L2() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).L2;
}

bool DebugControlRegister::L3() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).L3;
}

void DebugControlRegister::SetRW0(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0B11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).RW0 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetRW1(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0B11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).RW1 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetRW2(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0B11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).RW2 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetRW3(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0B11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).RW3 = value;
    registers_.Set(index_, reg_value);
}

std::uintptr_t DebugControlRegister::RW0() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).RW0;
}

std::uintptr_t DebugControlRegister::RW1() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).RW1;
}

std::uintptr_t DebugControlRegister::RW2() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).RW2;
}

std::uintptr_t DebugControlRegister::RW3() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).RW3;
}

void DebugControlRegister::SetLEN0(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0b11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).LEN0 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetLEN1(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0b11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).LEN1 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetLEN2(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0b11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).LEN2 = value;
    registers_.Set(index_, reg_value);
}

void DebugControlRegister::SetLEN3(const std::uintptr_t value) noexcept {
    assert(0 <= value && value <= 0b11);

    auto reg_value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(reg_value).LEN3 = value;
    registers_.Set(index_, reg_value);
}

std::uintptr_t DebugControlRegister::LEN0() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).LEN0;
}

std::uintptr_t DebugControlRegister::LEN1() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).LEN1;
}

std::uintptr_t DebugControlRegister::LEN2() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).LEN2;
}

std::uintptr_t DebugControlRegister::LEN3() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR7&>(value).LEN3;
}

void DebugControlRegister::SetL0(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(value).L0 = set;
    registers_.Set(index_, value);
}

void DebugControlRegister::SetL1(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(value).L1 = set;
    registers_.Set(index_, value);
}

void DebugControlRegister::SetL2(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(value).L2 = set;
    registers_.Set(index_, value);
}

void DebugControlRegister::SetL3(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR7&>(value).L3 = set;
    registers_.Set(index_, value);
}