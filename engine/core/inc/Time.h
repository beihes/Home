#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

namespace engine::core {
	struct FrameTime {
		Uint64 ns = 0;		  ///< @brief 每帧时间，单位纳秒
		double ms = 0.0;       ///< @brief 每帧时间，单位毫秒
		double second = 0.0;   ///< @brief 每帧时间，单位秒
	};
	class Time final {
	public:
		Time() = default;
		~Time() = default;
	public:
		//删除拷贝和移动构造函数及赋值运算符
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;
		Time(Time&&) = delete;
		Time& operator=(Time&&) = delete;
	public:
		bool StartTimer(Uint32 interval);
		bool StopTimer();
	public:
		void Update();
	public:
		SDL_TimerID GetTimerID() const;
		bool GetTimerState() const;
		void SetTimerState(bool midState);
	public:
		Uint64 GetTime();
		Uint64 GetFrameTime();
		double GetFrameTimeMs();
		double GetFrameTimeSecond();
		double GetFrame();
	private:
		// Add member variables and methods as needed
		SDL_TimerID timerID = 0;
		Uint64 currentTime = 0;
		Uint64 nowTime = 0;
		FrameTime frameTime{};
		double frame = 0.0;    ///< @brief 帧数
		bool timerState = false;
	private:
		static Uint32 TimerCallbackFunction(void* userdata, SDL_TimerID timerID, Uint32 interval);
	};
}