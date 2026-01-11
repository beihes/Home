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

#include <string>
#include <optional>

namespace engine::render {
	class Sprite final {
	public:
		Sprite() = default;
		Sprite(const std::string& textureId, const std::optional<SDL_FRect>& srcRect = std::nullopt, bool isFlipped = false);
		~Sprite() = default;
	public:
		void SetTextureId(const std::string& textureId);
		void SetSrcRect(const std::optional<SDL_FRect>& srcRect);
		void SetFlipped(bool isFlipped);
	public:
		const std::string& GetTextureId()const;
		const std::optional<SDL_FRect> GetSrcRect()const;
		const bool IsFlipped() const;
	private:
		std::string textureId_;
		std::optional<SDL_FRect> srcRect_;//纹理中要绘制的部分
		bool isFlipped_ = false;//是否水平翻转
	};
}
