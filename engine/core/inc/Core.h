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
#include <vector>

namespace engine::input {
	class InputManager;
}

namespace engine::physics {
	class PhysicsEngine;
}

namespace engine::render {
	class Camera;
    class Renderer;
    class TextRenderer;
}

namespace engine::resource {
	class ResourceManager;
}

namespace engine::scene {
	class SceneManager;
}

namespace engine::core {
	class Config;
	class Context;
	class Time;
	class Core final {
	public:
		Core() = default;
		~Core() = default;
	public:
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(Core&&) = delete;
	public:
		[[nodiscard]] bool Init();
		bool Init_SDL();
		bool Init_Config();
		bool Init_Context();
		bool Init_Time();
		bool Init_Renderer();
        bool Init_Camera();
        bool Init_TextRenderer();
        bool Init_ResourceManager();
		bool Init_InputManager();
		bool Init_SceneManager();
		[[nodiscard]] bool Init_PhysicsEngine();
	public:
		void Input(const SDL_Event& event);
		void Update();
		void Render();
		void Close();
	public:
		// Add member variables and methods as needed
		std::unique_ptr<Config> config_;
		std::unique_ptr<Context> context_;
		std::unique_ptr<input::InputManager> inputManager_;
		std::unique_ptr<physics::PhysicsEngine> physicsEngine_;
		std::unique_ptr<render::Camera> camera_;
        std::unique_ptr<render::Renderer> renderer_;
        std::unique_ptr<render::TextRenderer> textRenderer_;
        std::unique_ptr<resource::ResourceManager> resourceManager_;
		std::unique_ptr<scene::SceneManager> sceneManager_;
		std::unique_ptr<Time> time_;
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{ nullptr, SDL_DestroyWindow };
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{ nullptr,SDL_DestroyRenderer };
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}

