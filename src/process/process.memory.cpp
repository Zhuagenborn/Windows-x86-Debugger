#include "process.h"
#include "error.h"

#include <cassert>
#include <format>
#include <stdexcept>


bool Process::ValidMemory(const std::uintptr_t address) const noexcept {
    std::byte data{};
    std::size_t read_size{ 0 };
    return ReadProcessMemory(handle_, reinterpret_cast<LPCVOID>(address), &data,
                             sizeof(data),
                             reinterpret_cast<SIZE_T*>(&read_size));
}

std::vector<std::byte> Process::WriteMemory(
    const std::uintptr_t address, const std::span<const std::byte> data,
    const bool safe) const {
    return safe ? WriteMemorySafe(address, data)
                : WriteMemoryUnsafe(address, data);
}

std::vector<std::byte> Process::WriteMemorySafe(
    const std::uintptr_t address, const std::span<const std::byte> data) const {
    const auto end = address + data.size();
    for (const auto& [_, breakpoint] : software_breakpoints_) {
        if (breakpoint.type == BreakpointType::Software
            && address <= breakpoint.address && breakpoint.address < end) {
            throw std::runtime_error{ std::format(
                "A software breakpoint {:#010x} is located in the memory "
                "address range.",
                breakpoint.address) };
        }
    }

    return WriteMemoryUnsafe(address, data);
}

std::vector<std::byte> Process::WriteMemoryUnsafe(
    const std::uintptr_t address, const std::span<const std::byte> data) const {
    std::vector<std::byte> origin{ data.size() };
    std::size_t written_size{ 0 };
    if (!WriteProcessMemory(handle_, reinterpret_cast<LPVOID>(address),
                            data.data(), data.size(),
                            reinterpret_cast<SIZE_T*>(&written_size))) {
        ThrowLastError();
    }

    return origin;
}

std::vector<std::byte> Process::ReadMemory(const std::uintptr_t address,
                                           const std::size_t size,
                                           const bool safe) const {
    return safe ? ReadMemorySafe(address, size)
                : ReadMemoryUnsafe(address, size);
}

std::vector<std::byte> Process::ReadMemorySafe(const std::uintptr_t address,
                                               const std::size_t size) const {
    auto data = ReadMemoryUnsafe(address, size);

    const auto end = address + size;
    for (const auto& [_, breakpoint] : software_breakpoints_) {
        if (breakpoint.type == BreakpointType::Software
            && address <= breakpoint.address && breakpoint.address < end) {
            const auto offset = breakpoint.address - address;
            assert(data[offset] == Int3);

            data[offset] = breakpoint.original_byte;
        }
    }

    return data;
}

std::vector<std::byte> Process::ReadMemoryUnsafe(const std::uintptr_t address,
                                                 const std::size_t size) const {
    std::vector<std::byte> data{ size };
    std::size_t read_size{ 0 };
    if (!ReadProcessMemory(handle_, reinterpret_cast<LPCVOID>(address),
                           data.data(), size,
                           reinterpret_cast<SIZE_T*>(&read_size))) {
        ThrowLastError();
    }

    return data;
}