#include "register.h"
#include "registers.h"


DebugStatusRegister::DebugStatusRegister(Registers& registers) noexcept :
    Register{ registers, RegisterIndex::DR6 } {}

void DebugStatusRegister::SetB0() noexcept {
    SetB0(true);
}

void DebugStatusRegister::SetB1() noexcept {
    SetB1(true);
}

void DebugStatusRegister::SetB2() noexcept {
    SetB2(true);
}

void DebugStatusRegister::SetB3() noexcept {
    SetB3(true);
}

void DebugStatusRegister::ResetB0() noexcept {
    SetB0(false);
}

void DebugStatusRegister::ResetB1() noexcept {
    SetB1(false);
}

void DebugStatusRegister::ResetB2() noexcept {
    SetB2(false);
}

void DebugStatusRegister::ResetB3() noexcept {
    SetB3(false);
}

bool DebugStatusRegister::B0() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR6&>(value).B0;
}

bool DebugStatusRegister::B1() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR6&>(value).B1;
}

bool DebugStatusRegister::B2() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR6&>(value).B2;
}

bool DebugStatusRegister::B3() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const DR6&>(value).B3;
}

void DebugStatusRegister::SetB0(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR6&>(value).B0 = set;
    registers_.Set(index_, value);
}

void DebugStatusRegister::SetB1(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR6&>(value).B1 = set;
    registers_.Set(index_, value);
}

void DebugStatusRegister::SetB2(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR6&>(value).B2 = set;
    registers_.Set(index_, value);
}

void DebugStatusRegister::SetB3(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<DR6&>(value).B3 = set;
    registers_.Set(index_, value);
}