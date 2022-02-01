#include "thread.h"
#include "register/registers.h"

#include <cassert>


void Thread::SetHardwareBreakpoint(const std::uintptr_t address,
                                   const HardwareBreakpointSlot slot,
                                   const HardwareBreakpointType type,
                                   const HardwareBreakpointSize size) const {
    Registers registers(handle_, CONTEXT_DEBUG_REGISTERS);
    switch (slot) {
        case HardwareBreakpointSlot::DR0: {
            registers.DR0.Set(address);
            registers.DR7.SetL0();
            registers.DR7.SetRW0(static_cast<std::uintptr_t>(type));
            registers.DR7.SetLEN0(static_cast<std::uintptr_t>(size));
            break;
        }
        case HardwareBreakpointSlot::DR1: {
            registers.DR1.Set(address);
            registers.DR7.SetL1();
            registers.DR7.SetRW1(static_cast<std::uintptr_t>(type));
            registers.DR7.SetLEN1(static_cast<std::uintptr_t>(size));
            break;
        }
        case HardwareBreakpointSlot::DR2: {
            registers.DR2.Set(address);
            registers.DR7.SetL2();
            registers.DR7.SetRW2(static_cast<std::uintptr_t>(type));
            registers.DR7.SetLEN2(static_cast<std::uintptr_t>(size));
            break;
        }
        case HardwareBreakpointSlot::DR3: {
            registers.DR3.Set(address);
            registers.DR7.SetL3();
            registers.DR7.SetRW3(static_cast<std::uintptr_t>(type));
            registers.DR7.SetLEN3(static_cast<std::uintptr_t>(size));
            break;
        }
        default: {
            assert(false);
        }
    }
}

void Thread::DeleteHardwareBreakpoint(const HardwareBreakpointSlot slot) const {
    Registers registers(handle_, CONTEXT_DEBUG_REGISTERS);
    switch (slot) {
        case HardwareBreakpointSlot::DR0: {
            registers.DR0.Reset();
            registers.DR7.ResetL0();
            break;
        }
        case HardwareBreakpointSlot::DR1: {
            registers.DR1.Reset();
            registers.DR7.ResetL1();
            break;
        }
        case HardwareBreakpointSlot::DR2: {
            registers.DR2.Reset();
            registers.DR7.ResetL2();
            break;
        }
        case HardwareBreakpointSlot::DR3: {
            registers.DR3.Reset();
            registers.DR7.ResetL3();
            break;
        }
        default: {
            assert(false);
        }
    }
}