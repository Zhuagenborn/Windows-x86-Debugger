#include "process.h"

#include <format>
#include <initializer_list>
#include <stdexcept>


void Process::SetSoftwareBreakpoint(const std::uintptr_t address,
                                    const bool single_shoot,
                                    BreakpointCallback callback) {
    if (!ValidMemory(address)) {
        throw std::runtime_error{ std::format(
            "{:#010x} is not a valid memory address.", address) };
    } else if (hardware_breakpoints_.contains(address)) {
        throw std::runtime_error{ std::format(
            "A hardware breakpoint is already located at {:#010x}.", address) };
    }

    std::byte original_byte{};
    SetInt3(address, &original_byte);

    software_breakpoints_.insert(
        { address, { address, original_byte, single_shoot } });

    if (callback) {
        breakpoint_callbacks_[{ BreakpointType::Software, address }] =
            std::move(callback);
    }
}

bool Process::DeleteSoftwareBreakpoint(const std::uintptr_t address) {
    if (const auto found = software_breakpoints_.find(address);
        found != software_breakpoints_.cend()) {
        const auto& breakpoint = found->second;

        DeleteInt3(breakpoint.address, breakpoint.original_byte);

        software_breakpoints_.erase(found);
        breakpoint_callbacks_.erase({ BreakpointType::Software, address });

        return true;

    } else {
        return false;
    }
}

std::optional<SoftwareBreakpoint> Process::FindSoftwareBreakpoint(
    const std::uintptr_t address) const noexcept {
    const auto found = software_breakpoints_.find(address);
    return found != software_breakpoints_.cend()
               ? std::make_optional<SoftwareBreakpoint>(found->second)
               : std::nullopt;
}

void Process::SetInt3(const std::uintptr_t address,
                      std::byte* const original_byte) const {
    if (original_byte) {
        *original_byte = ReadMemoryUnsafe(address, sizeof(std::byte))[0];
    }

    WriteMemoryUnsafe(address, std::initializer_list{ int_3 });
}

void Process::DeleteInt3(const std::uintptr_t address,
                         const std::byte original_byte) const {
    WriteMemoryUnsafe(address, std::initializer_list{ original_byte });
}