#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <iostream>
#include <optional>

namespace engine::component {
    class TransformComponent;
}

namespace engine::render {
    class Camera final {
    public:
        Camera() = default;
        Camera(const SDL_FRect& viewRect = { 0,0,0,0 }, const std::optional<SDL_FRect>& limitRect = std::nullopt);
        ~Camera() = default;
    public:
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;

    public:
        void Update(float deltaTime);
        void Move(const SDL_FPoint& offset);

    public:
        SDL_FPoint WorldToScreen(const SDL_FPoint& worldPoint) const;
        // 世界坐标转屏幕坐标，考虑视差因子
        SDL_FPoint WorldToScreen(const SDL_FPoint& worldPoint, const SDL_FPoint& scrollFactor) const;
        SDL_FPoint ScreenToWorld(const SDL_FPoint& screenPoint) const;

    public:
        void SetPostion(const SDL_FPoint& positon);
        void SetVeiwSize(const SDL_FPoint& viewSize);
        void SetLimitRect(const SDL_FRect& limitRect);
        void SetTarget(component::TransformComponent* target);

    public:
        SDL_FPoint GetPosition()const;
        SDL_FPoint GetViewSize()const;
        const SDL_FRect& GetViewRect()const;
        const std::optional<SDL_FRect>& GetLimitRect()const;
        component::TransformComponent* GetTarget()const;
    private:
        void ClampPosition();//限制相机在边界内

    public:
        // 视窗大小和相机左上角的位置
        SDL_FRect viewRect_;
        float smoothSpeed_ = 5.0f;    
        // 相机的移动范围
        std::optional<SDL_FRect> limitRect_;
        // 相机跟随的目标
        component::TransformComponent* target_ = nullptr;
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}
