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

#include "./Component.h"
#include "render/inc/Sprite.h"
#include "utils/rect/inc/Rect.h"
#include <string>

namespace engine::component {
    class TransformComponent;

    /**
     * @brief 在背景中渲染可滚动纹理的组件，以创建视差效果。
     *
     * 该组件根据相机的位置和滚动因子来移动纹理。
     */
    class ParallaxComponent final : public Component {
        friend class engine::object::Object;
    public:
        /**
         * @brief 构造函数
         * @param textureId 背景纹理的资源 ID。
         * @param scrollFactor 控制背景相对于相机移动速度的因子。
         *                      (0, 0) 表示完全静止。
         *                      (1, 1) 表示与相机完全同步移动。
         *                      (0.5, 0.5) 表示以相机一半的速度移动。
         */
        ParallaxComponent(const std::string& textureId, SDL_FPoint scrollFactor, SDL_BPoint repeat);

        // --- 设置器 ---
        void SetSprite(const render::Sprite& sprite) { this->sprite_ = sprite; }  ///< @brief 设置精灵对象
        void SetScrollFactor(const SDL_FPoint& factor) { this->scrollFactor_ = factor; }  ///< @brief 设置滚动速度因子
        void SetRepeat(const SDL_BPoint& repeat) { this->repeat_ = repeat; }              ///< @brief 设置是否重复
        void SetHidden(bool hidden) { this->isHidden_ = hidden; }                        ///< @brief 设置是否隐藏（不渲染）

        // --- 获取器 ---
        const render::Sprite& GetSprite() const { return this->sprite_; }          ///< @brief 获取精灵对象
        const SDL_FPoint& GetScrollFactor() const { return this->scrollFactor_; }          ///< @brief 获取滚动速度因子
        const SDL_BPoint& GetRepeat() const { return this->repeat_; }                      ///< @brief 获取是否重复
        bool IsHidden() const { return this->isHidden_; }                                  ///< @brief 获取是否隐藏（不渲染）

    protected:
        // 核心循环函数覆盖
        void Update(float, core::Context&) override {}     // 必须实现纯虚函数，留空
        void Init() override;
        void Render(core::Context& context) override;
    private:
        TransformComponent* transform_ = nullptr;   ///< @brief 缓存变换组件
        render::Sprite sprite_;             ///< @brief 精灵对象
        SDL_FPoint scrollFactor_;                   ///< @brief 滚动速度因子 (0=静止, 1=随相机移动, <1=比相机慢)
        SDL_BPoint repeat_;                         ///< @brief 是否沿着X和Y轴周期性重复
        bool isHidden_ = false;                    ///< @brief 是否隐藏（不渲染）
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };

} // namespace engine::component