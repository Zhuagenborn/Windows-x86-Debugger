#include "register.h"
#include "registers.h"


Register::Register(Registers& registers, const RegisterIndex index) noexcept :
    registers_{ registers }, index_{ index } {}


Register& Register::operator=(const std::uintptr_t value) noexcept {
    Set(value);
    return *this;
}

Register& Register::operator++() noexcept {
    this->operator+=(1);
    return *this;
}

Register Register::operator++(int) noexcept {
    const Register ret{ *this };
    this->operator+=(1);
    return ret;
}

Register& Register::operator--() noexcept {
    this->operator-=(1);
    return *this;
}

Register Register::operator--(int) noexcept {
    const Register ret{ *this };
    this->operator-=(1);
    return ret;
}

Register& Register::operator+=(const std::uintptr_t value) noexcept {
    registers_.Set(index_, registers_.Get(index_) + value);
    return *this;
}

Register& Register::operator-=(const std::uintptr_t value) noexcept {
    registers_.Set(index_, registers_.Get(index_) - value);
    return *this;
}

bool operator==(const Register& register1, const Register& register2) noexcept {
    return register1.Get() == register2.Get();
}

bool operator!=(const Register& register1, const Register& register2) noexcept {
    return !(register1 == register2);
}

void Register::Set(const std::uintptr_t value) noexcept {
    registers_.Set(index_, value);
}

void Register::Reset() noexcept {
    Set(0);
}

std::uintptr_t Register::Get() const noexcept {
    return registers_.Get(index_);
}