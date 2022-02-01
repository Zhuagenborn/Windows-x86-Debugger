#include "debugger.h"
#include "register/registers.h"

#include <cassert>
#include <functional>


void Debugger::OnException(const EXCEPTION_DEBUG_INFO& details) {
    static const std::unordered_map<
        std::uint32_t,
        std::function<void(Debugger&, const EXCEPTION_RECORD&, bool)>>
        handlers{ { STATUS_BREAKPOINT, &Debugger::OnBreakpoint },
                  { STATUS_SINGLE_STEP, &Debugger::OnSingleStep },
                  { STATUS_ACCESS_VIOLATION, &Debugger::OnAccessViolation } };

    const auto& record = details.ExceptionRecord;
    const auto first_chance = details.dwFirstChance == 1;

    cbPreException(record, first_chance);

    if (handlers.contains(record.ExceptionCode)) {
        handlers.at(record.ExceptionCode)(*this, record, first_chance);
    }

    if (continue_status_ == DBG_EXCEPTION_NOT_HANDLED) {
        cbUnhandledException(record, first_chance);
    }
}


void Debugger::OnSingleStep(const EXCEPTION_RECORD& record,
                            const bool first_chance) {
    auto& thread = DebuggedThread();

    if (thread.InternalStepping()) {
        thread.ResetInternalStepping();
        continue_status_ = DBG_CONTINUE;

        thread.ExecuteInternalStepCallback();
    }

    if (thread.SingleStepping()) {
        thread.ResetSingleStepping();
        continue_status_ = DBG_CONTINUE;

        cbStep(thread);

        thread.ExecuteSingleStepCallbacks();

    } else {
        OnHardwareBreakpoint(
            reinterpret_cast<std::uintptr_t>(record.ExceptionAddress));
    }
}

void Debugger::OnBreakpoint(const EXCEPTION_RECORD& record,
                            const bool first_chance) {
    auto& process = DebuggedProcess();
    auto& thread = DebuggedThread();

    const auto found = process.FindSoftwareBreakpoint(
        reinterpret_cast<std::uintptr_t>(record.ExceptionAddress));

    if (!found && !process.HasHitSystemBreakpoint()) {
        process.HitSystemBreakpoint();
        continue_status_ = DBG_CONTINUE;

        cbSystemBreakpoint(process);

    } else {
        const auto& breakpoint = *found;
        Registers(thread.Handle(), CONTEXT_CONTROL).EIP.Set(breakpoint.address);
        process.DeleteInt3(breakpoint.address, breakpoint.original_byte);
        continue_status_ = DBG_CONTINUE;

        cbBreakpoint(breakpoint);

        if (breakpoint.address == thread.Entry()) {
            cbEntryBreakpoint(process);
        }

        if (breakpoint.single_shoot) {
            process.DeleteSoftwareBreakpoint(breakpoint.address);
        } else {
            thread.InternalStep([this, breakpoint]() {
                if (DebuggedProcess().FindSoftwareBreakpoint(
                        breakpoint.address)) {
                    DebuggedProcess().SetInt3(breakpoint.address);
                }
            });
        }

        process.ExecuteBreakpointCallback(
            { BreakpointType::Software, breakpoint.address });
    }
}

void Debugger::OnAccessViolation(const EXCEPTION_RECORD& record,
                                 const bool first_chance) {}

void Debugger::OnHardwareBreakpoint(const std::uintptr_t address) {
    auto& process = DebuggedProcess();
    auto& thread = DebuggedThread();

    Registers registers(thread.Handle(), CONTEXT_DEBUG_REGISTERS);
    const auto& dr6 = registers.DR6;
    HardwareBreakpointSlot slot{};
    if (address == registers.DR0.Get() || dr6.B0()) {
        slot = HardwareBreakpointSlot::DR0;
    } else if (address == registers.DR1.Get() || dr6.B1()) {
        slot = HardwareBreakpointSlot::DR1;
    } else if (address == registers.DR2.Get() || dr6.B2()) {
        slot = HardwareBreakpointSlot::DR2;
    } else if (address == registers.DR3.Get() || dr6.B3()) {
        slot = HardwareBreakpointSlot::DR3;
    } else {
        return;
    }

    const auto found = process.FindHardwareBreakpoint(address);
    assert(found);

    const auto& breakpoint = *found;
    assert(breakpoint.slot == slot);

    continue_status_ = DBG_CONTINUE;

    cbBreakpoint(breakpoint);

    thread.DeleteHardwareBreakpoint(slot);

    if (breakpoint.single_shoot) {
        process.DeleteHardwareBreakpoint(address);
    } else {
        thread.InternalStep([this, breakpoint]() {
            if (DebuggedProcess().FindHardwareBreakpoint(breakpoint.address)) {
                DebuggedThread().SetHardwareBreakpoint(
                    breakpoint.address, breakpoint.slot, breakpoint.access,
                    breakpoint.size);
            }
        });
    }

    process.ExecuteBreakpointCallback({ BreakpointType::Hardware, address });
}