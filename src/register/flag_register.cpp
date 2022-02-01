#include "register.h"
#include "registers.h"

#include <cassert>
#include <functional>
#include <unordered_map>


FlagRegister::FlagRegister(Registers& registers_) noexcept :
    Register{ registers_, RegisterIndex::EFLAGS } {}

void FlagRegister::SetCF() noexcept {
    SetCF(true);
}

void FlagRegister::SetPF() noexcept {
    SetPF(true);
}

void FlagRegister::SetAF() noexcept {
    SetAF(true);
}

void FlagRegister::SetZF() noexcept {
    SetZF(true);
}

void FlagRegister::SetSF() noexcept {
    SetSF(true);
}

void FlagRegister::SetTF() noexcept {
    SetTF(true);
}

void FlagRegister::SetIF() noexcept {
    SetIF(true);
}

void FlagRegister::SetDF() noexcept {
    SetDF(true);
}

void FlagRegister::SetOF() noexcept {
    SetOF(true);
}

void FlagRegister::ResetCF() noexcept {
    SetCF(false);
}

void FlagRegister::ResetPF() noexcept {
    SetPF(false);
}

void FlagRegister::ResetAF() noexcept {
    SetAF(false);
}

void FlagRegister::ResetZF() noexcept {
    SetZF(false);
}

void FlagRegister::ResetSF() noexcept {
    SetSF(false);
}

void FlagRegister::ResetTF() noexcept {
    SetTF(false);
}

void FlagRegister::ResetIF() noexcept {
    SetIF(false);
}

void FlagRegister::ResetDF() noexcept {
    SetDF(false);
}

void FlagRegister::ResetOF() noexcept {
    SetOF(false);
}

bool FlagRegister::CF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).CF;
}

bool FlagRegister::PF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).PF;
}

bool FlagRegister::AF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).AF;
}

bool FlagRegister::ZF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).ZF;
}

bool FlagRegister::SF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).SF;
}

bool FlagRegister::TF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).TF;
}

bool FlagRegister::IF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).IF;
}

bool FlagRegister::DF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).DF;
}

bool FlagRegister::OF() const noexcept {
    const auto value{ registers_.Get(index_) };
    return reinterpret_cast<const Flags&>(value).OF;
}

void FlagRegister::SetCF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).CF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetPF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).PF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetAF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).AF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetZF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).ZF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetSF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).SF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetTF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).TF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetIF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).IF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetDF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).DF = set;
    registers_.Set(index_, value);
}

void FlagRegister::SetOF(const bool set) noexcept {
    auto value{ registers_.Get(index_) };
    reinterpret_cast<Flags&>(value).OF = set;
    registers_.Set(index_, value);
}

void FlagRegister::Set(const Flag flag) noexcept {
    Set(flag, true);
}

void FlagRegister::Reset(const Flag flag) noexcept {
    Set(flag, false);
}

void FlagRegister::Set(const Flag flag, const bool set) noexcept {
    static const std::unordered_map<Flag, std::function<void(FlagRegister&, bool)>> handlers{
        { Flag::AF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetAF) },
        { Flag::CF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetCF) },
        { Flag::DF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetDF) },
        { Flag::IF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetIF) },
        { Flag::OF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetOF) },
        { Flag::SF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetSF) },
        { Flag::PF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetPF) },
        { Flag::TF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetTF) },
        { Flag::ZF, static_cast<void (FlagRegister::*)(bool)>(&FlagRegister::SetZF) }
    };

    assert(handlers.contains(flag));

    return handlers.at(flag)(*this, set);
}

bool FlagRegister::Get(const Flag flag) const noexcept {
    static const std::unordered_map<Flag, std::function<bool(const FlagRegister&)>> handlers{
        { Flag::AF, &FlagRegister::AF },
        { Flag::CF, &FlagRegister::CF },
        { Flag::DF, &FlagRegister::DF },
        { Flag::IF, &FlagRegister::IF },
        { Flag::OF, &FlagRegister::OF },
        { Flag::SF, &FlagRegister::SF },
        { Flag::PF, &FlagRegister::PF },
        { Flag::TF, &FlagRegister::TF },
        { Flag::ZF, &FlagRegister::ZF }
    };

    assert(handlers.contains(flag));

    return handlers.at(flag)(*this);
}