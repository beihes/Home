#include "../inc/Sprite.h"

namespace engine::render {
	Sprite::Sprite(const std::string& textureId, const std::optional<SDL_FRect>& srcRect, bool isFlipped) :textureId_(textureId), srcRect_(srcRect), isFlipped_(isFlipped)
	{
	}
	void Sprite::SetTextureId(const std::string& textureId)
	{
		this->textureId_ = textureId;
	}
	void Sprite::SetSrcRect(const std::optional<SDL_FRect>& srcRect)
	{
		this->srcRect_ = srcRect_;
	}
	void Sprite::SetFlipped(bool isFlipped)
	{
		this->isFlipped_ = isFlipped;
	}
	const std::string& Sprite::GetTextureId() const
	{
		return this->textureId_;
	}
	const std::optional<SDL_FRect> Sprite::GetSrcRect() const
	{
		return this->srcRect_;
	}
	const bool Sprite::IsFlipped()const
	{
		return this->isFlipped_;
	}
}
