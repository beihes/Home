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

#include "Component.h"
#include "engine/render/inc/Sprite.h"
#include "utils/rect/inc/Rect.h"
#include <iostream>
#include <optional>

namespace engine::core {
    class Context;
}

namespace engine::resource {
    class ResourceManager;
}

namespace engine::component {
    class TransformComponent;
    class SpriteComponent final : public component::Component {
        friend class object::Object;//友元不能继承，必须每个子类单独添加
    public:
        SpriteComponent() = default;
        SpriteComponent(const std::string& textureId,
            resource::ResourceManager& resourceManager,
            utils::Alignment alignment = utils::Alignment::NONE,
            std::optional<SDL_FRect> sourceRectOpt = std::nullopt,
            bool isFlipped = false);
        SpriteComponent(render::Sprite&& sprite,
            resource::ResourceManager& resourceManager,
            utils::Alignment alignment = utils::Alignment::NONE);
        ~SpriteComponent() override = default;
    public:
        //删除拷贝和移动构造函数及赋值运算符
        SpriteComponent(const SpriteComponent&) = delete;
        SpriteComponent& operator=(const SpriteComponent&) = delete;
        SpriteComponent(SpriteComponent&&) = delete;
        SpriteComponent& operator=(SpriteComponent&&) = delete;
    public:
        void UpdateOffset();//更新偏移量（根据当前的alignment_和spriteSize_计算offset_）
    public:
        const render::Sprite& GetSprite()const { return this->sprite_; }
        const std::string& GetTextureId()const { return this->sprite_.GetTextureId(); }
        bool isFlipped()const { return this->sprite_.IsFlipped(); }
        bool isHidden()const { return this->isHidden_; }
        const SDL_FPoint& GetSpriteSize()const { return this->spriteSize_; }
        const SDL_FPoint& GetOffset()const { return this->offset_; }
        utils::Alignment GetAlignment()const { return this->alignment_; }
    public:
        void SetSpriteById(const std::string& textureId, const std::optional<SDL_FRect>& sourceRectOpt = std::nullopt);
        void SetFlipped(bool isFlipped) { this->sprite_.SetFlipped(isFlipped); }
        void SetHidden(bool isHidden) { this->isHidden_ = isHidden; }
        void SetSourceRect(const std::optional<SDL_FRect>& sourceRectOpt);
        void SetAlignment(utils::Alignment anchor);
    private:
        void UpdateSpriteSize();
    protected:
        //Component虚函数覆盖
        void Init()override;
        void Update(float, core::Context&)override {}
        void Render(core::Context& context)override;
        void Clean()override {}
    private:
        resource::ResourceManager* resourceManager_ = nullptr;
        TransformComponent* transform_ = nullptr;
        render::Sprite sprite_;
        utils::Alignment alignment_ = utils::Alignment::NONE;//对齐方式
        SDL_FPoint spriteSize_ = { 0.f,0.f };//精灵尺寸
        SDL_FPoint offset_ = { 0,0 };//偏移量
        bool isHidden_ = false;//是否隐藏
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}
