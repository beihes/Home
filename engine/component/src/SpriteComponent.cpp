#include "../inc/SpriteComponent.h"
#include "../inc/TransformComponent.h"
#include "core/inc/Context.h"
#include "object/inc/Object.h"
#include "render/inc/Renderer.h"
#include "resource/inc/ResourceManager.h"
#include "utils/rect/inc/Rect.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::component
{
	SpriteComponent::SpriteComponent(const std::string& textureId, resource::ResourceManager& resourceManager, utils::Alignment alignment, std::optional<SDL_FRect> sourceRectOpt, bool isFlipped)
		:resourceManager_(&resourceManager), sprite_(textureId, sourceRectOpt, isFlipped), alignment_(alignment)
	{
		if (!this->resourceManager_)
		{
			log::Critical("[{}]创建SpriteManager时ResourceManager为空！此组件将无效", this->GetClassName());
		}
		log::Trace("[{}]创建SpriteComponent，纹理ID：{}", this->GetClassName(), textureId);
	}
	SpriteComponent::SpriteComponent(render::Sprite&& sprite, resource::ResourceManager& resourceManager, utils::Alignment alignment)
		:resourceManager_(&resourceManager), sprite_(std::move(sprite)), alignment_(alignment_)
	{
		if (!this->resourceManager_)
		{
			log::Critical("[{}]创建SpriteManager时ResourceManager为空！此组件将无效", this->GetClassName());
		}
		log::Trace("[{}]创建SpriteComponent，纹理ID：{}", this->GetClassName(), this->sprite_.GetTextureId());
	}
	void SpriteComponent::UpdateOffset()
	{
		if (this->spriteSize_.x <= 0 || this->spriteSize_.y <= 0)
		{
			offset_ = { 0,0 };
			return;
		}
		auto scale = this->transform_->Get_Scale();
		switch (this->alignment_)
		{
			case utils::Alignment::TOP_LEFT:		this->offset_ = SDL_FPoint{ 0,0 }; break;
			case utils::Alignment::TOP_CENTER:		this->offset_ = SDL_FPoint{ -this->spriteSize_.x / 2.f,0 } * scale; break;
			case utils::Alignment::TOP_RIGHT:		this->offset_ = SDL_FPoint{ -this->spriteSize_.x,0 } * scale; break;
			case utils::Alignment::CENTER_LEFT:		this->offset_ = SDL_FPoint{ 0,-this->spriteSize_.y / 2.f } * scale; break;
			case utils::Alignment::CENTER:			this->offset_ = SDL_FPoint{ -this->spriteSize_.x / 2.f,-this->spriteSize_.y / 2.f } * scale; break;
			case utils::Alignment::CENTER_RIGHT:	this->offset_ = SDL_FPoint{ -this->spriteSize_.x,-this->spriteSize_.y / 2.f } * scale; break;
			case utils::Alignment::BOTTOM_LEFT:		this->offset_ = SDL_FPoint{ 0,-this->spriteSize_.y } * scale; break;
			case utils::Alignment::BOTTOM_CENTER:	this->offset_ = SDL_FPoint{ -this->spriteSize_.x / 2.f,-this->spriteSize_.y } * scale; break;
			case utils::Alignment::BOTTOM_RIGHT:	this->offset_ = SDL_FPoint{ -this->spriteSize_.x,-this->spriteSize_.y } * scale; break;
			case utils::Alignment::NONE:
			default:								break;
		}
	}
	void SpriteComponent::SetSpriteById(const std::string& textureId, const std::optional<SDL_FRect>& sourceRectOpt)
	{
		this->sprite_.SetTextureId(textureId);
		this->sprite_.SetSrcRect(sourceRectOpt);
		this->UpdateSpriteSize();
		this->UpdateOffset();

	}
	void SpriteComponent::SetSourceRect(const std::optional<SDL_FRect>& sourceRectOpt)
	{
		this->sprite_.SetSrcRect(sourceRectOpt);
		this->UpdateSpriteSize();
		this->UpdateOffset();
	}
	void SpriteComponent::SetAlignment(utils::Alignment anchor)
	{
		this->alignment_ = anchor;
		this->UpdateOffset();
	}
	void SpriteComponent::UpdateSpriteSize()
	{
		if (!this->resourceManager_)
		{
			log::Error("[{}]ResourceManager为空！无法获取纹理尺寸", this->GetClassName());
			return;
		}
		if (this->sprite_.GetSrcRect().has_value())
		{
			const auto& srcRect = this->sprite_.GetSrcRect().value();
			this->spriteSize_ = { srcRect.x,srcRect.y };
		}
		else {
			this->spriteSize_ = this->resourceManager_->GetTextureSize(this->sprite_.GetTextureId());
		}
	}
	void SpriteComponent::Init()
	{
		if (!this->owner_) {
			log::Error("[{}]在初始化前未设置所有者", this->GetClassName());
			return;
		}
		this->transform_ = this->owner_->GetComponent<TransformComponent>();
		if (!this->transform_) {
			log::Warn("[{}]Object '{}'上的SpriteComponent需要一个TransformComponent，但未找到", this->GetClassName(), this->owner_->GetName());
			return;
		}
		this->UpdateSpriteSize();
		this->UpdateOffset();
	}
	void SpriteComponent::Render(core::Context& context)
	{
		if (this->isHidden_ || !this->transform_ || !resourceManager_)
		{
			return;
		}
		const SDL_FPoint& pos = this->transform_->Get_Position_Pixels() + this->offset_;
		const SDL_FPoint& scale = this->transform_->Get_Scale();
		float rotationDegrees = this->transform_->Get_Rotation();
		context.GetRenderer().DrawSprite(context.GetCamera(), this->sprite_, pos, scale, rotationDegrees);
	}
}
