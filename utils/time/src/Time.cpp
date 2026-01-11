#include "../inc/Time.h"
#include <utility>

namespace utils::time
{
	std::optional<SDL_DateTime> SDL_GetCurrentDateTime()
	{
		SDL_Time nowTime;  // 获取当前纳秒时间戳
		SDL_DateTime dateTime;
		if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) {
			return std::nullopt;
		}
		return dateTime;
	}
	std::string SDL_DateToString()
	{
		SDL_Time nowTime;  // 获取当前纳秒时间戳
		SDL_DateTime dateTime;
		if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) {
			return "Unknown";
		}
		return SDL_DateToString(dateTime);
	}
	std::string SDL_DateToString(const SDL_DateTime& dateTime)
	{
		char date[16];
		SDL_snprintf(date, sizeof(date), "%04d-%02d-%02d", dateTime.year, dateTime.month, dateTime.day);
		return std::string(date);
	}
	std::string SDL_TimeToString()
	{
		SDL_Time nowTime;  // 获取当前纳秒时间戳
		SDL_DateTime dateTime;
		if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) {
			return "Unknown";
		}
		return SDL_TimeToString(dateTime);
	}
	std::string SDL_TimeToString(const SDL_DateTime& dateTime)
	{
		char time[20];
		SDL_snprintf(time, sizeof(time), "%02d:%02d:%02d.%03d", dateTime.hour, dateTime.minute, dateTime.second, SDL_NS_TO_MS(dateTime.nanosecond));
		return std::string(time);
	}
	std::string SDL_DateTimeToString()
	{
		SDL_Time nowTime;  // 获取当前纳秒时间戳
		SDL_DateTime dateTime;
		if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) {
			return "Unknown";
		}
		return SDL_DateTimeToString(dateTime);
	}
	std::string SDL_DateTimeToString(const SDL_DateTime& dateTime)
	{
		char time[35];
		SDL_snprintf(time, sizeof(time), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
					 dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second, SDL_NS_TO_MS(dateTime.nanosecond));
		return std::string(time);
	}
}