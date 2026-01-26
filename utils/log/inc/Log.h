#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <format>

namespace utils::log {
    void Init(SDL_LogPriority priority);
    bool Set_LogFile(const std::string& fileDirectory);
    bool CreateLogFile();
    bool Open_LogFile();
    bool Close_LogFile();
    const char* SDL_LogPriorityToString(SDL_LogPriority priority);
    std::string GetHeaderString(SDL_LogPriority priority);
    void LogOutputCallBack(void* userdata, int category, SDL_LogPriority priority, const char* message);

    template <typename... Args>
    void Debug(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_DEBUG < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_DEBUG) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_ERROR < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_ERROR) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_INFO < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_INFO) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Trace(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_TRACE < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_TRACE) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_TRACE, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Verbose(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_VERBOSE < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_VERBOSE) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Warn(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_WARN < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_WARN) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, msg.c_str(), nullptr);
    }

    template <typename... Args>
    void Critical(std::format_string<Args...> fmt, Args&&... args) {
        if (SDL_LOG_PRIORITY_CRITICAL < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
        std::string msg = GetHeaderString(SDL_LOG_PRIORITY_CRITICAL) + std::format(fmt, std::forward<Args>(args)...);
        SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, msg.c_str(), nullptr);
    }
}