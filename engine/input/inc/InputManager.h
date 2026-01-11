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

#include <entt/entt.hpp>
#include <iostream>
#include <array>
#include <string>
#include <unordered_map>
#include <variant>

namespace engine::core{
	class Config;
}

namespace engine::input {
	enum class ActionState {
		PRESSED,//动作在本帧按下
		HELD,//动作被持续按下
		RELEASED,//动作在本帧释放
		INACTIVE//动作未激活
	};
	class InputManager final {
	public:
		InputManager() = default;
		InputManager(SDL_Renderer* renderer, const core::Config* config);
		~InputManager() = default;
	public:
		entt::sink<entt::sigh<void()>> OnAction(const std::string& actionName, ActionState actionState = ActionState::PRESSED);
	public:
		void Update(const SDL_Event& event);
	public:
		bool IsActionDown(const std::string& actionName)const;
		bool IsActionPressed(const std::string& actionName)const;
		bool isActionReleased(const std::string& actionName)const;
		bool ShouldQuit()const;
		const SDL_FPoint& GetMousePosition()const;//获取屏幕坐标
		SDL_FPoint GetLogicalMousePosition()const;//获取逻辑坐标
	public:
		void SetShouldQuit(bool shouldQuit);
	private:
		void ProcessEvent(const SDL_Event& event);
		void InitMappings(const core::Config* config);
		void UpdateActionState(const std::string& actionName, bool isInputActive, bool isRepeatEvent);
		SDL_Scancode GetScancodeFromString(const std::string& keyName);
		Uint8 GetMouseButtonUint8FromString(const std::string& buttonName);
	public:
		SDL_Renderer* renderer_ = nullptr;
		bool shouldQuit_ = false;
		SDL_FPoint mousePosition_;
		std::unordered_map<std::string, std::array<entt::sigh<void()>, 3>> actionsToFunc_;
		std::unordered_map<std::variant<SDL_Scancode, Uint8>, std::vector<std::string>> inputToActions_ = {};//从建盘或鼠标获取动作名称列表
		std::unordered_map<std::string, ActionState> actionStates_ = {};//存储每个动作当前状态
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
