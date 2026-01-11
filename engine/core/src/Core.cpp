#include "../inc/Core.h"
#include "../inc/Config.h"
#include "../inc/Context.h"
#include "../inc/Time.h"
#include "component/inc/Component.h"
#include "component/inc/SpriteComponent.h"
#include "component/inc/TransformComponent.h"
#include "physics/inc/PhysicsEngine.h"
#include "render/inc/Camera.h"
#include "render/inc/Renderer.h"
#include "render/inc/TextRenderer.h"
#include "render/inc/Sprite.h"
#include "resource/inc/ResourceManager.h"
#include "input/inc/InputManager.h"
#include "object/inc/Object.h"
#include "scene/inc/Scene.h"
#include "scene/inc/SceneManager.h"
#include "utils/rect/inc/Rect.h"
#include "app/scene/inc/AppScene.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::core {
	bool Core::Init()
	{
		log::Init(SDL_LOG_PRIORITY_TRACE);
        // log::Set_LogFile("log");
        log::Info("<----应用启动---->");
		if (!this->Init_Config())return false;
		if (!this->Init_SDL())return false;
		if (!this->Init_ResourceManager())return false;
		if (!this->Init_Renderer())return false;
        if (!this->Init_Camera())return false;
        if (!this->Init_TextRenderer())return false;
        if (!this->Init_InputManager())return false;
		if (!this->Init_Time())return false;
		if (!this->Init_PhysicsEngine())return false;
		if (!this->Init_Context())return false;
		if (!this->Init_SceneManager())return false;
		auto scene = std::make_unique<app::scene::AppScene>("AppScene", *this->context_, *this->sceneManager_);
		this->sceneManager_->RequestPushScene(std::move(scene));
		log::Trace("[{}]Core初始化成功", this->GetClassName());
		return true;
	}
	bool Core::Init_SDL()
	{
		SDL_SetAppMetadata("Example HUMAN READABLE NAME", "1.0", "com.bhs.cpptest");
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
			log::Error("[{}]Couldn't initialize SDL: {}",this->GetClassName(), SDL_GetError());
			return false;
		}
#if defined(SDL_PLATFORM_ANDROID)
		this->config_->isFullScreen = true;
#endif // defined(SDL_PLATFORM_ANDROID)
		this->window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
			SDL_CreateWindow(
				this->config_->windowTitle.c_str(),
				this->config_->windowRect.w,
				this->config_->windowRect.h,
				this->config_->isFullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE
			),
			SDL_DestroyWindow
		);
		if (!this->window)
			return false;
		this->renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
			SDL_CreateRenderer(
				this->window.get(),
				nullptr
			),
			SDL_DestroyRenderer
		);
		if (!this->renderer)
			return false;
		return true;
	}
	bool Core::Init_Config()
	{
		this->config_ = std::make_unique<Config>("assets/config");
		if (!this->config_)
		{
			return false;
		}
		return true;
	}
	bool Core::Init_Context()
	{
		this->context_ = std::make_unique<Context>(*this->camera_, *this->inputManager_, *this->physicsEngine_, *this->renderer_,*this->textRenderer_, *this->resourceManager_);
		if (!this->context_)
		{
			log::Error("[{}]初始化上下文失败", this->GetClassName());
			return false;
		}
		return true;
	}
	bool Core::Init_Time()
	{
		this->time_ = std::make_unique<Time>();
		return this->time_->StartTimer(1000.0 / this->config_->GetFPS());
	}
	bool Core::Init_Renderer()
	{
		renderer_ = std::make_unique<render::Renderer>(renderer.get(), resourceManager_.get());
		if (!renderer_)
		{
			log::Error("[{}]初始化渲染器失败", this->GetClassName());
			return false;
		}
		return true;
	}
	bool Core::Init_Camera()
	{
		SDL_FRect midRect = { 0,0,float(this->config_->windowRect.w),float(this->config_->windowRect.h) };
		this->camera_ = std::make_unique<render::Camera>(midRect);
		if (!this->camera_)
		{
			log::Error("[{}]初始化相机失败", this->GetClassName());
			return false;
		}
		return true;
    }
    bool Core::Init_TextRenderer()
    {
        this->textRenderer_ = std::make_unique<render::TextRenderer>(renderer.get(), resourceManager_.get());
        if (!this->textRenderer_)
        {
            log::Error("[{}]初始化文本渲染器失败", this->GetClassName());
            return false;
        }
        return true;
    }
    bool Core::Init_ResourceManager()
	{
		resourceManager_ = std::make_unique<resource::ResourceManager>(this->renderer.get());
		if (!resourceManager_)
		{
			log::Error("[{}]resourceManager_ init failed", this->GetClassName());
			return false;
		}
		return true;
	}
	bool Core::Init_InputManager()
	{
		this->inputManager_ = std::make_unique<input::InputManager>(this->renderer.get(), this->config_.get());
		if (!this->inputManager_)
		{
			return false;
		}
		return true;
	}
	bool Core::Init_SceneManager()
	{
		this->sceneManager_ = std::make_unique<scene::SceneManager>(*this->context_);
		if (!this->sceneManager_)
		{
			log::Error("[{}]初始化场景管理器失败", this->GetClassName());
			return false;
		}
		log::Trace("[{}]场景管理器初始化成功", this->GetClassName());
		return true;
	}
	bool Core::Init_PhysicsEngine()
	{
		this->physicsEngine_ = std::make_unique<physics::PhysicsEngine>();
		if (!this->physicsEngine_)
		{
			log::Error("[{}]初始化物理引擎失败", this->GetClassName());
			return false;
		}
		log::Trace("[{}]物理引擎初始化成功", this->GetClassName());
		return true;
	}
	void Core::Input(const SDL_Event& event)
	{
		//这里进入中断，不支持更新
		this->inputManager_->Update(event);
	}
	void Core::Update()
	{
		//这里的是没有中断，得到input的消息就开始更新，所以不需要放在处理事件的函数里
		this->sceneManager_->Input();
		double targetTime = 1.0 / this->config_->GetFPS();
		this->sceneManager_->Update((this->time_->GetFrameTimeSecond() < (5 * targetTime)) ? this->time_->GetFrameTimeSecond() : targetTime);
	}
	void Core::Render()
	{
		this->renderer_->ClearScreen();
		this->sceneManager_->Render();
		this->renderer_->Present();
	}
	void Core::Close()
	{
		this->sceneManager_->Close();
		this->resourceManager_.reset();
	}
}


