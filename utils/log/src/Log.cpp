#include "../inc/Log.h"
#include "utils/file/inc/File.h"
#include "utils/time/inc/Time.h"
#include <memory>
#include <regex>

namespace utils::log
{
	static std::unique_ptr<SDL_IOStream, decltype(&SDL_CloseIO)> ioStream = std::unique_ptr<SDL_IOStream, decltype(&SDL_CloseIO)>(nullptr, SDL_CloseIO);
	static std::string fileDirectory_ = "";
	static std::string fileName_ = "";
	static std::string filePath_ = "";
	void Init(SDL_LogPriority priority)
	{
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INVALID, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_TRACE, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_VERBOSE, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_DEBUG, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_WARN, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_ERROR, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_CRITICAL, "");
		SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_COUNT, "");
		SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, priority);
	}
	bool Set_LogFile(const std::string& fileDirectory)
	{
		fileDirectory_ = fileDirectory;
		fileName_ = time::SDL_DateToString() + ".log";
		filePath_ = fileDirectory_ + '/' + fileName_;
		return Open_LogFile();
	}
	bool CreateLogFile()
	{
		ioStream.reset(SDL_IOFromFile(filePath_.c_str(), "a+"));
		if (!ioStream)
		{
			Error("日志文件创建错误: {}", SDL_GetError());
			return false;
		}
		SDL_SetLogOutputFunction(LogOutputCallBack, ioStream.get());
		Info("日志文件创建成功: {}", filePath_.c_str());
		return true;
	}
	bool Open_LogFile()
	{
		if (ioStream)
		{
			Warn("日志文件已经打开");
			return true;
		}
		if (!file::CreateDirectory(fileDirectory_))
		{
			Error("日志文件创建失败");
			return false;
		}
		Info("日志目录就绪: {}", fileDirectory_);
		return CreateLogFile();
	}
	bool Close_LogFile()
	{
		if (!ioStream) { return true; }
		if (!ioStream.release())
		{
			Error("日志文件关闭失败");
			return false;
		}
		SDL_SetLogOutputFunction(SDL_GetDefaultLogOutputFunction(), nullptr);
		Info("日志文件关闭: {}", filePath_);
		return true;
	}
	const char* SDL_LogPriorityToString(SDL_LogPriority priority)
	{
		switch (priority) {
			case SDL_LOG_PRIORITY_TRACE:    return "[\033[35mTRACE\033[0m] ";
			case SDL_LOG_PRIORITY_VERBOSE: 	return "[\033[90mVERBOSE\033[0m] ";
			case SDL_LOG_PRIORITY_DEBUG:    return "[\033[36mDEBUG\033[0m] ";
			case SDL_LOG_PRIORITY_INFO:     return "[\033[32mINFO\033[0m] ";
			case SDL_LOG_PRIORITY_WARN:     return "[\033[33mWARN\033[0m] ";
			case SDL_LOG_PRIORITY_ERROR:    return "[\033[31mERROR\033[0m] ";
			case SDL_LOG_PRIORITY_CRITICAL: return "[\033[41;97mCRITICAL\033[0m] ";
			default: return "";
		}
	}

	std::string GetHeaderString(SDL_LogPriority priority)
	{
		std::string header;
		header.reserve(64);
		header += '[';
		header += time::SDL_DateTimeToString();
		header += "] ";
		header += SDL_LogPriorityToString(priority);
		return header;
	}
	void LogOutputCallBack(void* userdata, int category, SDL_LogPriority priority, const char* message)
	{
		auto ioStream = (SDL_IOStream*)userdata;
		if (ioStream) {
			std::string plainMsg = std::regex_replace(message, std::regex("\033\\[[0-9;]*m"), "");
			SDL_WriteIO(ioStream, plainMsg.c_str(), plainMsg.size());	// 写入内容
			SDL_WriteIO(ioStream, "\n", 1);          // 换行
		}
		SDL_GetDefaultLogOutputFunction()(userdata, category, priority, message);
	}
}