#include "process.h"

#include <algorithm>
#include <format>
#include <stdexcept>


bool Process::DeleteHardwareBreakpoint(const std::uintptr_t address) {
    const auto found = hardware_breakpoints_.find(address);
    if (found == hardware_breakpoints_.cend()) {
        return false;
    }

    const auto& breakpoint = found->second;
    std::ranges::for_each(threads_, [&breakpoint](auto& pair) {
        auto& [_, thread] = pair;
        thread.DeleteHardwareBreakpoint(breakpoint.slot);
    });

    hardware_breakpoints_.erase(found);
    breakpoint_callbacks_.erase({ BreakpointType::Hardware, address });
    hardware_breakpoint_slots_[breakpoint.slot] = nullptr;
    return true;
}

std::optional<HardwareBreakpoint> Process::FindHardwareBreakpoint(
    const std::uintptr_t address) const noexcept {
    const auto found = hardware_breakpoints_.find(address);
    return found != hardware_breakpoints_.cend()
               ? std::make_optional<HardwareBreakpoint>(found->second)
               : std::nullopt;
}

bool Process::FindFreeHardwareBreakpointSlot(
    HardwareBreakpointSlot& slot) const noexcept {
    for (auto i = 0; i != hardware_breakpoint_slot_count; ++i) {
        slot = static_cast<HardwareBreakpointSlot>(i);
        if (!hardware_breakpoint_slots_.at(slot)) {
            return true;
        }
    }

    return false;
}

void Process::SetHardwareBreakpoint(const std::uintptr_t address,
                                    const HardwareBreakpointSlot slot,
                                    const HardwareBreakpointType type,
                                    const HardwareBreakpointSize size,
                                    bool single_shoot,
                                    BreakpointCallback callback) {
    if (!ValidMemory(address)) {
        throw std::runtime_error{ std::format(
            "{:#010x} is not a valid memory address.", address) };
    } else if (hardware_breakpoints_.contains(address)) {
        throw std::runtime_error{ std::format(
            "A hardware breakpoint is already located at {:#010x}.", address) };
    }

    for (auto& [_, thread] : threads_) {
        thread.SetHardwareBreakpoint(address, slot, type, size);
    }

    hardware_breakpoints_.insert(
        { address, { address, slot, type, size, single_shoot } });
    hardware_breakpoint_slots_[slot] = &hardware_breakpoints_.at(address);

    if (callback) {
        breakpoint_callbacks_[{ BreakpointType::Hardware, address }] =
            std::move(callback);
    }
}