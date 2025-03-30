/**
 * @file error.h
 * @brief Error handling.
 *
 * @author Chen Zhenshuo (chenzs108@outlook.com)
 * @author Liu Guowen (liu.guowen@outlook.com)
 * @version 1.0
 * @date 2021-07-10
 * @par GitHub
 * https://github.com/czs108
 * @par
 * https://github.com/lgw1995
 *
 * @see https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes
 */

#pragma once

//! Throw a @p std::system_error exception containing the last-error.
[[noreturn]] void ThrowLastError();