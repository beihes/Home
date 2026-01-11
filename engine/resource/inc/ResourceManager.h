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
#include <memory>

namespace engine::resource {
	class AudioManager;
	class FontManager;
	class TextureManager;
	class ResourceManager {
	public:
		ResourceManager() = default;
		explicit ResourceManager(SDL_Renderer* renderer);
		~ResourceManager();
	public:
		//删除拷贝
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	public:
		void Init();
		void Clear();
	public:
		void* GetAudio(const std::string& filePath);
		TTF_Font* GetFont(const std::string& filePath, float fontSize);
		SDL_Texture* GetTexture(const std::string& filePath);
		SDL_FPoint GetTextureSize(const std::string& textureId);
	public:
		bool Unload_Font(const std::string& filePath, float fontSize);
		bool Unload_Texture(const std::string& filePath);
	public:
		std::unique_ptr<AudioManager> audioManager_;
		std::unique_ptr<FontManager> fontManager_;
		std::unique_ptr<TextureManager> textureManager_;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
