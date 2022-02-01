#include "memory"

#include <Windows.h>


std::uint32_t RemoveWriteAccess(const std::uint32_t access) {
    const std::uint32_t low = access & 0xFF;
    const std::uint32_t high = access & 0xFFFFFF00;
    switch (low) {
        case PAGE_EXECUTE: {
            return high | PAGE_READONLY;
        }
        case PAGE_EXECUTE_WRITECOPY:
        case PAGE_EXECUTE_READWRITE:
        case PAGE_EXECUTE_READ: {
            return high | (low >> 4);
        }
        default: {
            return access;
        }
    }
}

std::uint32_t RemoveExecuteAccess(const std::uint32_t access) {
    const std::uint32_t low = access & 0xFF;
    const std::uint32_t high = access & 0xFFFFFF00;
    switch (low) {
        case PAGE_READWRITE:
        case PAGE_EXECUTE_READWRITE: {
            return high | (low >> 1);
        }
        default: {
            return access;
        }
    }
}