#include "breakpoint.h"


Breakpoint::Breakpoint(const std::uintptr_t address, const BreakpointType type,
                       const bool single_shoot) noexcept :
    address{ address }, type{ type }, single_shoot{ single_shoot } {}


SoftwareBreakpoint::SoftwareBreakpoint(const std::uintptr_t address,
                                       const std::byte original_byte,
                                       const bool single_shoot) noexcept :
    Breakpoint{ address, BreakpointType::Software, single_shoot },
    original_byte{ original_byte } {}


HardwareBreakpoint::HardwareBreakpoint(const std::uintptr_t address,
                                       const HardwareBreakpointSlot slot,
                                       const HardwareBreakpointType access,
                                       const HardwareBreakpointSize size,
                                       const bool single_shoot) noexcept :
    Breakpoint{ address, BreakpointType::Hardware, single_shoot },
    slot{ slot },
    access{ access },
    size{ size } {}