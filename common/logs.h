#ifndef EVM_LOGS_H
#define EVM_LOGS_H

#include <cstddef>
#include <iostream>
#include <sstream>

namespace evm::logger {

static inline void PrintLogRecursive(const char *file_name, size_t line, std::ostringstream &message)
{
    std::cout << "[" << file_name << ", line " << line << "] " << message.str() << std::endl;
}

static inline void PrintErrRecursive(const char *file_name, size_t line, std::ostringstream &message)
{
    std::cerr << "[" << file_name << ", line " << line << "] " << message.str() << std::endl;
}

template <typename T, typename... Args>
static inline void PrintLogRecursive(const char *file_name, size_t line, std::ostringstream &message, T value,
                                     const Args &...args)
{
    message << value;
    PrintLogRecursive(file_name, line, message, args...);
}

template <typename T, typename... Args>
static inline void PrintErrRecursive(const char *file_name, size_t line, std::ostringstream &message, T value,
                                     const Args &...args)
{
    message << value;
    PrintErrRecursive(file_name, line, message, args...);
}

template <typename... Args>
static inline void PrLog(const char *file_name, size_t line, const Args &...args)
{
    std::ostringstream message;
    PrintLogRecursive(file_name, line, message, args...);
}

template <typename... Args>
static inline void PrErr(const char *file_name, size_t line, const Args &...args)
{
    std::ostringstream message;
    PrintErrRecursive(file_name, line, message, args...);
}

} // namespace evm::logger

#ifdef NDEBUG
#define PrintLog(...)
#else
#define PrintLog(...) evm::logger::PrLog(__FILE__, __LINE__, __VA_ARGS__)
#endif

#define PrintErr(...) evm::logger::PrErr(__FILE__, __LINE__, __VA_ARGS__)

#endif // EVM_LOGS_H
