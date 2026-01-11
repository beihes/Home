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

namespace utils::file
{
	bool CreateDirectory(const std::string& filePath);
	//bool CreateLogFile(const std::string& filePath);
	bool ReadFile(const std::string& filePath, std::string& outContent);
}