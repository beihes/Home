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
#include <memory>
#include <functional>
#include <utility>
#include <bit>

namespace engine::resource {
	using FontKey = std::pair<std::string, float>;
	struct FontKeyHash {
		std::size_t operator()(const FontKey& fontKey)const {
			std::hash<std::string> stringHash;
			std::hash<uint32_t> intHash;
			return stringHash(fontKey.first) ^ intHash(std::bit_cast<uint32_t>(fontKey.second));
		}
	};
	class FontManager {
		friend class ResourceManager;
	public:
		FontManager();
		~FontManager();
	public:
		//删除拷贝
		FontManager(const FontManager&) = delete;
		FontManager& operator=(const FontManager&) = delete;
		FontManager(FontManager&&) = delete;
		FontManager& operator=(FontManager&&) = delete;
	public:
		void Init();
		void Clear();
	public:
		TTF_Font* Load_Font_From_File(const std::string& filePath, float fontSize);
		TTF_Font* GetFont(const std::string& filePath, float fontSize);
		bool UnloadFont(const std::string& filePath, float fontSize);
	private:
		struct TTF_FontDelete {
			void operator()(TTF_Font* font) {
				if (!font)
					return;
				TTF_CloseFont(font);
			}
		};
		std::unordered_map<FontKey, std::unique_ptr<TTF_Font, TTF_FontDelete>, FontKeyHash> fonts_;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
