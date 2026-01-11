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
#include <fmt/core.h>

namespace utils::log
{
	void Init(SDL_LogPriority priority);
	bool Set_LogFile(const std::string& fileDirectory);
	bool CreateLogFile();
	bool Open_LogFile();
	bool Close_LogFile();
	const char* SDL_LogPriorityToString(SDL_LogPriority priority);
	std::string GetHeaderString(SDL_LogPriority priority);
	void LogOutputCallBack(void* userdata, int category, SDL_LogPriority priority, const char* message);

	template <typename... Args>
	void Debug(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_DEBUG < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_DEBUG) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, msg.c_str(), nullptr);
    }
    
    template <typename... Args>
	void Error(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_ERROR < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_ERROR) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, msg.c_str(), nullptr);
	}

    template <typename... Args>
    void Info(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_INFO < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_INFO) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, msg.c_str(), nullptr);
	}
	
    template <typename... Args>
	void Trace(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_TRACE < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_TRACE) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_TRACE, msg.c_str(), nullptr);
	}

    template <typename... Args>
    void Verbose(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_VERBOSE < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_VERBOSE) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE, msg.c_str(), nullptr);
	}
	
    template <typename... Args>
	void Warn(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_WARN < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_WARN) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, msg.c_str(), nullptr);
	}

    template <typename... Args>
    void Critical(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (SDL_LOG_PRIORITY_CRITICAL < SDL_GetLogPriority(SDL_LOG_CATEGORY_APPLICATION)) { return; }
		std::string msg = GetHeaderString(SDL_LOG_PRIORITY_CRITICAL) + fmt::format(fmt, std::forward<Args>(args)...);
		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, msg.c_str(), nullptr);
	}
}