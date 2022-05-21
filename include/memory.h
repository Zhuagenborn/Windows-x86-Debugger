/**
 * @file memory.h
 * @brief Memory properties.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-11-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>


constexpr std::size_t memory_page_size = 0x1000;

enum class MemoryType {
    Access = 0B0001,
    Read = 0B0010,
    Write = 0B0100,
    Execute = 0B1000
};

//! A memory range, including a start address and an end address.
using MemoryRange = std::pair<std::uintptr_t, std::uintptr_t>;

/**
 * Check if the first memory range comes before the second memory range.
 */
constexpr bool operator<(const MemoryRange& range1,
                         const MemoryRange& range2) noexcept {
    return range1.second < range2.first;
}

/**
 * Remove the write permission from memory protection.
 */
std::uint32_t RemoveWriteAccess(std::uint32_t access);

/**
 * Remove the execute permission from memory protection.
 */
std::uint32_t RemoveExecuteAccess(std::uint32_t access);