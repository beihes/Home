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

#include <iostream>

namespace engine::render {
	class Camera;
    class Renderer;
    class TextRenderer;
}

namespace engine::physics {
	class PhysicsEngine;
}

namespace engine::resource {
	class ResourceManager;
}

namespace engine::input {
	class InputManager;
}

namespace engine::core {
	class Context final {
	public:
		Context() = default;
		Context(render::Camera& camera, input::InputManager& inputManager, physics::PhysicsEngine& physicsEngine, render::Renderer& renderer, render::TextRenderer& textRenderer, resource::ResourceManager& resourceManager);
		~Context();
	public:
		//删除拷贝和移动构造函数及赋值运算符
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;
		Context(Context&&) = delete;
		Context& operator=(Context&&) = delete;
	public:
		input::InputManager& GetInputManager()const { return this->inputManager_; }
		render::Camera& GetCamera()const { return this->camera_; }
		physics::PhysicsEngine& GetPhysicsEngine()const { return this->physicsEngine_; }
		render::Renderer& GetRenderer()const { return this->renderer_; }
        render::TextRenderer& GetTextRenderer()const { return this->textRenderer_; }
        resource::ResourceManager& GetResourceManager()const { return this->resourceManager_; }
    private:
		//使用引用确保每个模块都有效，使用时不需要检查指针是否为空
		input::InputManager& inputManager_;
		render::Camera& camera_;
		physics::PhysicsEngine& physicsEngine_;
        render::Renderer& renderer_;
        render::TextRenderer& textRenderer_;
        resource::ResourceManager& resourceManager_;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}