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

#include "utils/rect/inc/Rect.h"
#include <iostream>
#include <optional>

namespace engine::resource{
	class ResourceManager;
}

namespace engine::render {
	class Camera;
	class Sprite;
	class Renderer final{
	public:
		Renderer() = default;
		Renderer(SDL_Renderer* renderer, resource::ResourceManager* resourceManager);
		~Renderer() = default;
	public:
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
	public:
		void DrawSprite(const Camera& camera, const Sprite& sprite, const SDL_FPoint& position, const SDL_FPoint& scale = { 1.0f,1.0f }, double angle = 0.0f);
		void DrawParallax(const Camera& camera, const Sprite& sprite, const SDL_FPoint& position, const SDL_FPoint& scrollFactor, const SDL_BPoint& repeat = { true,true }, const SDL_FPoint& scale = { 1.0f,1.0f });
		void DrawUISprite(const Sprite& sprite, const SDL_FPoint& position, const std::optional<SDL_FPoint>& size = std::nullopt);
	public:
		void SetDrawColor(const SDL_Color& color);
		void SetDrawColor(const SDL_FColor& color);
		void ClearScreen();//清屏
		void Present();//更新屏幕
	public:
		SDL_Renderer* GetRenderer() const;
	private:
		std::optional<SDL_FRect> GetSpriteSrcRect(const Sprite& sprite);//获取精灵的原矩形
		bool isRectInViewRect(const Camera& camera, const SDL_FRect& rect);//判断矩形是否在视界矩形内;
	public:
		SDL_Renderer* renderer_ = nullptr;
		resource::ResourceManager* resourceManager_ = nullptr;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
