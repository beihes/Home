#include "../inc/InputManager.h"
#include "core/inc/Config.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::input
{
	InputManager::InputManager(SDL_Renderer* renderer, const core::Config* config)
	{
		InitMappings(config);
		if (!renderer)
		{
			log::Error("[{}]renderer为空指针",this->GetClassName());
			return;
		}
		this->renderer_ = renderer;
		SDL_GetMouseState(&this->mousePosition_.x, &this->mousePosition_.y);
		log::Trace("[{}]初始鼠标位置({},{})", this->GetClassName(), this->mousePosition_.x, this->mousePosition_.y);
    }
    
    entt::sink<entt::sigh<void()>> InputManager::OnAction(const std::string& actionName, ActionState actionState)
	{
		return this->actionsToFunc_[actionName].at(static_cast<size_t>(actionState));
	}

    void InputManager::Update(const SDL_Event& event)
    {
		for (auto& [actionName, state] : actionStates_)
		{
			if (state == ActionState::PRESSED) {
				state = ActionState::HELD;
			} else if (state == ActionState::RELEASED) { state = ActionState::INACTIVE; }
		}
		this->ProcessEvent(event);
		for (auto& [actionNameId, state] : this->actionStates_)
		{
			if (state != ActionState::INACTIVE)
			{
				if (auto it = this->actionsToFunc_.find(actionNameId);it != this->actionsToFunc_.end())
				{
					it->second.at(static_cast<size_t>(state)).publish();
				}
			}
		}
	}

    bool InputManager::IsActionDown(const std::string& actionName) const
    {
		if (auto it = this->actionStates_.find(actionName);it != this->actionStates_.end())
		{
			return it->second == ActionState::PRESSED || it->second == ActionState::HELD;
		}
		return false;
	}

    bool InputManager::IsActionPressed(const std::string& actionName) const
    {
		if (auto it = this->actionStates_.find(actionName); it != this->actionStates_.end())
		{
			return it->second == ActionState::PRESSED ;
		}
		return false;
	}

    bool InputManager::isActionReleased(const std::string& actionName) const
    {
		if (auto it = this->actionStates_.find(actionName); it != this->actionStates_.end())
		{
			return it->second == ActionState::RELEASED;
		}
		return false;
	}

    bool InputManager::ShouldQuit() const
    {
		return this->shouldQuit_;
	}

    const SDL_FPoint& InputManager::GetMousePosition() const
    {
		return this->mousePosition_;
    }
    
    SDL_FPoint InputManager::GetLogicalMousePosition() const
	{
		SDL_FPoint midPoint;
		SDL_RenderCoordinatesFromWindow(this->renderer_, this->mousePosition_.x, this->mousePosition_.y, &midPoint.x, &midPoint.y);
		return midPoint;
	}

    void InputManager::SetShouldQuit(bool shouldQuit)
    {
		this->shouldQuit_ = shouldQuit;
	}

    void InputManager::ProcessEvent(const SDL_Event& event)
    {
		switch (event.type) {
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP: {
			SDL_Scancode scancode = event.key.scancode;
			bool isDown = event.key.down;
			bool isRepeat = event.key.repeat;
			auto it = this->inputToActions_.find(scancode);
			if (it != this->inputToActions_.end())
			{
				const std::vector<std::string>& associatedActions = it->second;
				for (const std::string& actionName : associatedActions)
				{
					this->UpdateActionState(actionName, isDown, isRepeat);
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			Uint8 button = event.button.button;
			bool isDown = event.button.down;
			auto it = this->inputToActions_.find(button);
			if (it!= this->inputToActions_.end())
			{
				const std::vector<std::string>& associatedActions = it->second;
				for (const std::string& actionName:associatedActions)
				{
					this->UpdateActionState(actionName, isDown, false);
				}
			}
			this->mousePosition_ = { event.button.x,event.button.y };
			break;
		}
		case SDL_EVENT_MOUSE_MOTION:
			this->mousePosition_ = { event.motion.x,event.motion.y };
			break;
		case SDL_EVENT_QUIT:
			shouldQuit_ = true;
			break;
		default:
			break;
		}
	}
	void InputManager::InitMappings(const core::Config* config)
	{
		log::Trace("初始化输入映射");
		if (!config)
		{
			log::Error("[{}]config为空指针", this->GetClassName());
			return;
		}
		auto actionsToKeyname = config->keyMap;
		this->inputToActions_.clear();
		this->actionStates_.clear();
		if (actionsToKeyname.find("MouseLeft") == actionsToKeyname.end())
		{
			log::Debug("[{}]配置中没有定义'MouseLeft'", this->GetClassName());
			actionsToKeyname["MouseLeft"] = { "MouseLeft" };
		}
		if (actionsToKeyname.find("MouseRight") == actionsToKeyname.end())
		{
			log::Debug("[{}]配置中没有定义'MouseRight'", this->GetClassName());
			actionsToKeyname["MouseRight"] = { "MouseRight" };
		}
        for (const auto& [actionName, keyNames] : actionsToKeyname)
        {
            actionStates_[actionName] = ActionState::INACTIVE;
            log::Trace("[{}]映射动作：{}", this->GetClassName(), actionName);
            for (const std::string& keyName : keyNames)
            {
                SDL_Scancode scancode = this->GetScancodeFromString(keyName);
                Uint8 mouseButton = this->GetMouseButtonUint8FromString(keyName);
                if (scancode != SDL_SCANCODE_UNKNOWN)
                {
                    this->inputToActions_[scancode].push_back(actionName);
                    log::Trace("[{}]映射按键：{}(SDL_Scancode：{})到动作：{}", this->GetClassName(), keyName, int(scancode), actionName);
                } else if (mouseButton != 0)
                {
                    this->inputToActions_[mouseButton].push_back(actionName);
                    log::Trace("[{}]映射鼠标按钮：{}(Uint8：{})到动作：{}", this->GetClassName(), keyName, mouseButton, actionName);
                } else
                {
                    log::Warn("[{}]输入映射警告：未知按钮名称'{}'到动作'{}'", this->GetClassName(), keyName, actionName);
                }
            }
        }
		log::Trace("[{}]输入映射初始化完成", this->GetClassName());
    }
    
    void InputManager::UpdateActionState(const std::string& actionName, bool isInputActive, bool isRepeatEvent)
	{
		auto it = this->actionStates_.find(actionName);
		if (it == this->actionStates_.end())
		{
			log::Warn("[{}]尝试更新未注册的动作状态：{}", this->GetClassName(), actionName);
			return;
		}
		if (isInputActive)
		{
			if (isRepeatEvent)it->second = ActionState::HELD;
			else it->second = ActionState::PRESSED;
		}
		else it->second = ActionState::RELEASED;
	}

    SDL_Scancode InputManager::GetScancodeFromString(const std::string& keyName)
    {
		return SDL_GetScancodeFromName(keyName.c_str());
	}

    Uint8 InputManager::GetMouseButtonUint8FromString(const std::string& buttonName)
    {
		if (buttonName == "MouseLeft")return SDL_BUTTON_LEFT;
		if (buttonName == "MouseMiddle")return SDL_BUTTON_MIDDLE;
		if (buttonName == "MouseRight")return SDL_BUTTON_RIGHT;
		if (buttonName == "MouseX1")return SDL_BUTTON_X1;
		if (buttonName == "MouseX2")return SDL_BUTTON_X2;
		return 0;
	}
}
