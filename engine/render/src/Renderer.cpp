#include "../inc/Renderer.h"
#include "../inc/Camera.h"
#include "../inc/Sprite.h"
#include "resource/inc/ResourceManager.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::render
{
	Renderer::Renderer(SDL_Renderer* renderer, resource::ResourceManager* resourceManager):renderer_(renderer),resourceManager_(resourceManager)
	{
		if (!this->renderer_)
		{
			log::Error("[{}]构造失败，SDL_Renderer为空",this->GetClassName());
			return;
		}
		if (!this->resourceManager_)
		{
			log::Error("[{}]构造失败，ResourceManager为空",this->GetClassName());
			return;
		}
		log::Trace("[{}]构造成功",this->GetClassName());
	}
	void Renderer::DrawSprite(const Camera& camera, const Sprite& sprite, const SDL_FPoint& position, const SDL_FPoint& scale, double angle)
	{
		auto texture = resourceManager_->GetTexture(sprite.GetTextureId());
		if (!texture)
		{
			log::Error("[{}]无法获取纹理，ID：{}", this->GetClassName(), sprite.GetTextureId());
			return;
		}
		auto srcRect = GetSpriteSrcRect(sprite);
		if (!srcRect.has_value())
		{
			log::Error("[{}]无法获取源矩形，ID：{}", this->GetClassName(), sprite.GetTextureId());
			return;
		}
		SDL_FPoint positionScreen = camera.WorldToScreen(position);
		float scaledW = srcRect.value().w * scale.x;
		float scaledH = srcRect.value().h * scale.y;
		float midLength = SDL_max(scaledW, scaledH);
		SDL_FRect destRect = { position.x + (scaledW - midLength) / 2,position.y + (scaledH - midLength) / 2,midLength,midLength };
		if (!isRectInViewRect(camera, destRect))return;
		destRect = { positionScreen.x ,positionScreen.y ,scaledW ,scaledH };
		if (!SDL_RenderTextureRotated(renderer_, texture, &srcRect.value(), &destRect, angle, nullptr, sprite.IsFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
		{
			log::Error("[{}]渲染纹理失败，ID:{}:{}", this->GetClassName(), sprite.GetTextureId(), SDL_GetError());
		}
	}
	void Renderer::DrawParallax(const Camera& camera, const Sprite& sprite, const SDL_FPoint& position, const SDL_FPoint& scrollFactor, const SDL_BPoint& repeat, const SDL_FPoint& scale)
	{
		auto texture = resourceManager_->GetTexture(sprite.GetTextureId());
		if (!texture)
		{
			log::Error("[{}]无法获取纹理，ID：{}", this->GetClassName(), sprite.GetTextureId());
			return;
		}
		auto srcRect = GetSpriteSrcRect(sprite);
		if (!srcRect.has_value())
		{
			log::Error("[{}]无法获取源矩形，ID：{}", this->GetClassName(), sprite.GetTextureId());
			return;
		}
		SDL_FPoint positionScreen = camera.WorldToScreen(position, scrollFactor);
		float scaledTextureW = srcRect.value().w * scale.x;
		float scaledTextureH = srcRect.value().h * scale.y;
		SDL_FPoint start, stop;
		SDL_FPoint viewSize = camera.GetViewSize();
		if (repeat.x)// X 方向
		{
			start.x = positionScreen.x - SDL_floorf(positionScreen.x / scaledTextureW) * scaledTextureW - scaledTextureW;
			stop.x = viewSize.x;
		}
		else
		{
			start.x = positionScreen.x;
			stop.x = positionScreen.x + scaledTextureW;
		}
		if (repeat.y)// Y 方向
		{
			start.y = positionScreen.y - SDL_floorf(positionScreen.y / scaledTextureH) * scaledTextureH - scaledTextureH;
			stop.y = viewSize.y;
		}
		else
		{
			start.y = positionScreen.y;
			stop.y = positionScreen.y + scaledTextureH;
		}
		// 平铺绘制
		for (float y = start.y; y < stop.y; y += scaledTextureH)
		{
			for (float x = start.x; x < stop.x; x += scaledTextureW)
			{
				SDL_FRect destRect = { x, y, scaledTextureW, scaledTextureH };
				if (!SDL_RenderTexture(renderer_, texture, nullptr, &destRect))
				{
					log::Error("[{}]渲染视差纹理失败，ID：{}：{}", this->GetClassName(), sprite.GetTextureId(), SDL_GetError());
					return;
				}
			}
		}
	}
	void Renderer::DrawUISprite(const Sprite& sprite, const SDL_FPoint& position, const std::optional<SDL_FPoint>& size)
	{
		auto texture = resourceManager_->GetTexture(sprite.GetTextureId());
		if (!texture)
		{
			log::Error("[{}]无法获取纹理，ID：{}",this->GetClassName(), sprite.GetTextureId());
			return;
		}
		auto srcRect = GetSpriteSrcRect(sprite);
		if (!srcRect.has_value())
		{
			log::Error("[{}]无法获取源矩形，ID：{}", this->GetClassName(), sprite.GetTextureId());
			return;
		}
		SDL_FRect destRect = { position.x,position.y,0,0 };
		if (size.has_value())
		{
			destRect.w = size.value().x;
			destRect.h = size.value().y;
		}
		else
		{
			destRect.w = srcRect.value().w;
			destRect.h = srcRect.value().h;
		}
		if (!SDL_RenderTextureRotated(renderer_, texture, &srcRect.value(), &destRect, 0, nullptr, sprite.IsFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
		{
			log::Error("[{}]渲染UI失败，ID:{}:{}", this->GetClassName(), sprite.GetTextureId(), SDL_GetError());
		}
	}
	void Renderer::SetDrawColor(const SDL_Color& color)
	{
		if (!SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.g, color.a))
		{
			log::Error("[{}]渲染颜色失败，{}", this->GetClassName(), SDL_GetError());
		}
	}
	void Renderer::SetDrawColor(const SDL_FColor& color)
	{
		if (!SDL_SetRenderDrawColorFloat(renderer_, color.r, color.g, color.g, color.a))
		{
			log::Error("[{}]渲染颜色失败，{}", this->GetClassName(), SDL_GetError());
		}
	}
	void Renderer::ClearScreen()
	{
		if (!SDL_RenderClear(renderer_))
		{
			log::Error("[{}]清除渲染器失败，{}", this->GetClassName(), SDL_GetError());
		}
	}
	void Renderer::Present()
	{
		SDL_RenderPresent(renderer_);
	}
	SDL_Renderer* Renderer::GetRenderer() const
	{
		return renderer_;
	}
	std::optional<SDL_FRect> Renderer::GetSpriteSrcRect(const Sprite& sprite)
	{
		SDL_Texture* texture = this->resourceManager_->GetTexture(sprite.GetTextureId());
		if (!texture)
		{
			log::Error("[{}]无法为ID：{}获取纹理", this->GetClassName(), sprite.GetTextureId());
			return std::nullopt;
		}
		auto srcRect = sprite.GetSrcRect();
		if (srcRect.has_value())
		{
			if (SDL_RectEmptyFloat(&srcRect.value()))
			{
				log::Error("[{}]原矩形尺寸无效，ID:{}", this->GetClassName(), sprite.GetTextureId());
				return std::nullopt;
			}
			return srcRect;
		}
		else
		{
			SDL_FRect result = { 0 };
			if (!SDL_GetTextureSize(texture, &result.w, &result.h))
			{
				log::Error("[{}]无法获取纹理尺寸，ID: {}", this->GetClassName(), sprite.GetTextureId());
				return std::nullopt;
			}
			return result;
		}
	}
	bool Renderer::isRectInViewRect(const Camera& camera, const SDL_FRect& rect)
	{
		return SDL_HasRectIntersectionFloat(&camera.GetViewRect(), &rect);
	}
}
