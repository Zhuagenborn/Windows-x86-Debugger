#include "error.h"

#include <Windows.h>

#include <system_error>


[[noreturn]] void ThrowLastError() {
    throw std::system_error{ static_cast<int>(GetLastError()),
                             std::system_category() };
}