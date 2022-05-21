#include "registers.h"
#include "error.h"

#include <cassert>
#include <cstring>


Registers::Registers(const HANDLE thread, const std::uint32_t context_flags) :
    EAX{ *this, RegisterIndex::EAX },
    EBX{ *this, RegisterIndex::EBX },
    ECX{ *this, RegisterIndex::ECX },
    EDX{ *this, RegisterIndex::EDX },
    EIP{ *this, RegisterIndex::EIP },
    ESI{ *this, RegisterIndex::ESI },
    EDI{ *this, RegisterIndex::EDI },
    ESP{ *this, RegisterIndex::ESP },
    EBP{ *this, RegisterIndex::EBP },
    EFLAGS{ *this },
    DR0{ *this, RegisterIndex::DR0 },
    DR1{ *this, RegisterIndex::DR1 },
    DR2{ *this, RegisterIndex::DR2 },
    DR3{ *this, RegisterIndex::DR3 },
    DR6{ *this },
    DR7{ *this },
    thread_{ thread } {
    context_.ContextFlags = context_flags;
    if (GetThreadContext(thread, &context_)) {
        original_context_ = context_;
    } else {
        ThrowLastError();
    }
}

Registers::~Registers() {
    if (thread_
        && std::memcmp(&context_, &original_context_, sizeof(CONTEXT)) != 0) {
        SetThreadContext(thread_, &context_);
    }
}

HANDLE Registers::Thread() const noexcept {
    return thread_;
}

void Registers::Set(const RegisterIndex index,
                    const std::uintptr_t value) noexcept {
    switch (index) {
        case RegisterIndex::EAX: {
            context_.Eax = value;
            break;
        }
        case RegisterIndex::EBX: {
            context_.Ebx = value;
            break;
        }
        case RegisterIndex::ECX: {
            context_.Ecx = value;
            break;
        }
        case RegisterIndex::EDX: {
            context_.Edx = value;
            break;
        }
        case RegisterIndex::EIP: {
            context_.Eip = value;
            break;
        }
        case RegisterIndex::ESP: {
            context_.Esp = value;
            break;
        }
        case RegisterIndex::EBP: {
            context_.Ebp = value;
            break;
        }
        case RegisterIndex::ESI: {
            context_.Esi = value;
            break;
        }
        case RegisterIndex::EDI: {
            context_.Edi = value;
            break;
        }
        case RegisterIndex::EFLAGS: {
            context_.EFlags = value;
            break;
        }
        case RegisterIndex::DR0: {
            context_.Dr0 = value;
            break;
        }
        case RegisterIndex::DR1: {
            context_.Dr1 = value;
            break;
        }
        case RegisterIndex::DR2: {
            context_.Dr2 = value;
            break;
        }
        case RegisterIndex::DR3: {
            context_.Dr3 = value;
            break;
        }
        case RegisterIndex::DR6: {
            context_.Dr6 = value;
            break;
        }
        case RegisterIndex::DR7: {
            context_.Dr7 = value;
            break;
        }
        default: {
            assert(false);
        }
    }
}

void Registers::Reset(const RegisterIndex index) noexcept {
    Set(index, 0);
}

std::uint32_t Registers::Get(const RegisterIndex index) const noexcept {
    switch (index) {
        case RegisterIndex::EAX: {
            return context_.Eax;
        }
        case RegisterIndex::EBX: {
            return context_.Ebx;
        }
        case RegisterIndex::ECX: {
            return context_.Ecx;
        }
        case RegisterIndex::EDX: {
            return context_.Edx;
        }
        case RegisterIndex::EIP: {
            return context_.Eip;
        }
        case RegisterIndex::ESP: {
            return context_.Esp;
        }
        case RegisterIndex::EBP: {
            return context_.Ebp;
        }
        case RegisterIndex::ESI: {
            return context_.Esi;
        }
        case RegisterIndex::EDI: {
            return context_.Edi;
        }
        case RegisterIndex::EFLAGS: {
            return context_.EFlags;
        }
        case RegisterIndex::DR0: {
            return context_.Dr0;
        }
        case RegisterIndex::DR1: {
            return context_.Dr1;
        }
        case RegisterIndex::DR2: {
            return context_.Dr2;
        }
        case RegisterIndex::DR3: {
            return context_.Dr3;
        }
        case RegisterIndex::DR6: {
            return context_.Dr6;
        }
        case RegisterIndex::DR7: {
            return context_.Dr7;
        }
        default: {
            assert(false);
            return 0;
        }
    }
}