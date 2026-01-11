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
#include <string>
#include <unordered_map>

namespace engine::resource {
	class TextureManager {
		friend class ResourceManager;
	public:
		TextureManager() = default;
		TextureManager(SDL_Renderer* renderer);
		~TextureManager();
	public:
		//删除拷贝
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager(TextureManager&&) = delete;
		TextureManager& operator=(TextureManager&&) = delete;
	public:
		void Init();
		void Clear();
	public:
		SDL_Texture* Load_Texture_From_File(const std::string& filePath);
		SDL_Texture* GetTexture(const std::string& filePath);
		bool Unload_Texture(const std::string& filePath);
	private:
		struct SDL_TextureDelete {
			void operator()(SDL_Texture* texture) {
				if (!texture)
					return;
				SDL_DestroyTexture(texture);
			}
		};
		std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDL_TextureDelete>> textures_;
		SDL_Renderer* renderer_ = nullptr;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
