#include "process.h"

#include <utility>


OptionalThread Process::FindThread(const std::uint32_t id) const noexcept {
    const auto found{ threads_.find(id) };
    return found != threads_.cend()
               ? OptionalThread{ const_cast<Thread&>(found->second) }
               : std::nullopt;
}

void Process::NewThread(Thread&& thread) noexcept {
    threads_.insert({ thread.Id(), std::move(thread) });
}

bool Process::RemoveThread(const std::uint32_t id) noexcept {
    return threads_.erase(id) != 0;
}