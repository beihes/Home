#include "../inc/Time.h"
#include "../inc/Config.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::core {
	bool Time::StartTimer(Uint32 interval)
	{
		if (timerState)
			return false;
		timerID = SDL_AddTimer(interval, this->TimerCallbackFunction, this);
		if (!this->timerID)
			return false;
		timerState = true;
		return true;
	}
	bool Time::StopTimer()
	{
		if (!SDL_RemoveTimer(timerID)) {
			log::Error("[{}]can not stop timer:%s", SDL_GetError());
			return false;
		}
		timerState = false;
		return true;
	}
	void Time::Update()
	{
		this->currentTime = this->nowTime;
		this->nowTime = SDL_GetTicksNS();
		this->frameTime.ns = this->nowTime - this->currentTime;
		this->frameTime.ms = this->frameTime.ns / 1000000.0;
		this->frameTime.second = this->frameTime.ms / 1000.0;
		this->frame = 1.0 / this->frameTime.second;
	}
	SDL_TimerID Time::GetTimerID() const
	{
		return this->timerID;
	}
	bool Time::GetTimerState() const
	{
		return this->timerState;
	}
	void Time::SetTimerState(bool midState)
	{
		this->timerState = midState;
	}
	Uint64 Time::GetTime()
	{
		return this->nowTime;
	}
	Uint64 Time::GetFrameTime()
	{
		return this->frameTime.ns;
	}
	double Time::GetFrameTimeMs()
	{
		return this->frameTime.ms;
	}
	double Time::GetFrameTimeSecond()
	{
		return this->frameTime.second;
	}
	double Time::GetFrame()
	{
		return this->frame;
	}
	Uint32 Time::TimerCallbackFunction(void* userdata, SDL_TimerID timerID, Uint32 interval)
	{
		auto midData = (Time*)userdata;
		midData->timerState = true;
		return interval;
	}
}