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
	class AudioManager {
		friend class ResourceManager;
	public:
		AudioManager() = default;
		~AudioManager();
	public:
		void Init();
		void Clear();
	public:
		struct TTF_FontDelete {
			void operator()(TTF_Font* font) {
				if (!font)
					return;
				TTF_CloseFont(font);
			}
		};
		std::unordered_map<std::string, std::unique_ptr<TTF_Font, TTF_FontDelete>> textures_;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
