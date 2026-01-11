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
#include <optional>

namespace utils::time {
    enum Week {
        MONDEY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY, WEEKDAY
    };
    [[nodiscard]] std::optional<SDL_DateTime> SDL_GetCurrentDateTime();
	std::string SDL_DateToString();
	std::string SDL_DateToString(const SDL_DateTime& dateTime);
	std::string SDL_TimeToString();
	std::string SDL_TimeToString(const SDL_DateTime& dateTime);
	std::string SDL_DateTimeToString();
	std::string SDL_DateTimeToString(const SDL_DateTime& dateTime);
} // namespace utils::time