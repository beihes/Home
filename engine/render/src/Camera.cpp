#include "../inc/Camera.h"
#include "component/inc/TransformComponent.h"
#include "utils/log/inc/Log.h"
#include "utils/rect/inc/Rect.h"

using namespace utils;
using namespace utils::rect;

namespace engine::render
{
    Camera::Camera(const SDL_FRect& viewRect, const std::optional<SDL_FRect>& limitRect)
        : viewRect_(viewRect), limitRect_(limitRect)
    {
        log::Trace("[{}]相机创建完成", this->GetClassName());
    }
    void Camera::Update(float deltaTime)
    {
        if (!this->target_)return;
        SDL_FPoint targetPos = this->target_->Get_Position_Pixels();
        SDL_FPoint desiredPos = targetPos - this->GetViewSize() / 2.f;
        auto distance = SDL_FPoint_distance(this->viewRect_, desiredPos);
        constexpr float SNAP_THRESHOLD = 1.0f; // 设置一个距离阈值  (constexpr: 编译时常量，避免每次调用都计算)
        if (distance < SNAP_THRESHOLD) {
            // 如果距离小于阈值，直接吸附到目标位置
            this->viewRect_.x = desiredPos.x;
            this->viewRect_.y = desiredPos.y;
        } else {
            // 否则，使用线性插值平滑移动   glm::mix(a,b,t): 在向量 a 和 b 之间进行插值，t 是插值因子，范围在0到1之间。
                                     // 公式: (b-a)*t + a;   t = 0 时结果为 a，t = 1 时结果为 b
            auto pos = SDL_FPoint_mix(SDL_FPoint{this->viewRect_.x,this->viewRect_.y}, desiredPos, this->smoothSpeed_ * deltaTime);
            pos = SDL_FPoint{ (float)SDL_round(pos.x), (float)SDL_round(pos.y) };    // 四舍五入到整数,省略的话偶尔会出现画面割裂
            this->viewRect_.x = pos.x;
            this->viewRect_.y = pos.y;
        }
        this->ClampPosition();
    }
    void Camera::Move(const SDL_FPoint& offset)
    {
        this->viewRect_.x += offset.x;
        this->viewRect_.y += offset.y;
        this->ClampPosition();
    }
    SDL_FPoint Camera::WorldToScreen(const SDL_FPoint& worldPoint) const
    {
        return { worldPoint.x - this->viewRect_.x, worldPoint.y - this->viewRect_.y };
    }
    SDL_FPoint Camera::WorldToScreen(const SDL_FPoint& worldPoint, const SDL_FPoint& scrollFactor) const
    {
        return { worldPoint.x - this->viewRect_.x * scrollFactor.x ,worldPoint.y - this->viewRect_.y * scrollFactor.y };
    }
    SDL_FPoint Camera::ScreenToWorld(const SDL_FPoint& screenPoint) const
    {
        return { screenPoint.x + this->viewRect_.x, screenPoint.y + this->viewRect_.y };
    }
    void Camera::SetPostion(const SDL_FPoint& positon)
    {
        this->viewRect_.x = positon.x;
        this->viewRect_.y = positon.y;
        this->ClampPosition();
    }
    void Camera::SetVeiwSize(const SDL_FPoint& viewSize)
    {
        this->viewRect_.w = viewSize.x;
        this->viewRect_.h = viewSize.y;
        this->ClampPosition();
    }
    void Camera::SetLimitRect(const SDL_FRect& limitRect)
    {
        this->limitRect_ = limitRect_;
        this->ClampPosition();
    }
    void Camera::SetTarget(component::TransformComponent* target)
    {
        this->target_ = target;
    }
    SDL_FPoint Camera::GetPosition()const
    {
        return { this->viewRect_.x,this->viewRect_.y };
    }
    SDL_FPoint Camera::GetViewSize()const
    {
        return { this->viewRect_.w,this->viewRect_.h };
    }
    const SDL_FRect& Camera::GetViewRect() const
    {
        return this->viewRect_;
    }
    const std::optional<SDL_FRect>& Camera::GetLimitRect()const
    {
        return this->limitRect_;
    }
    component::TransformComponent* Camera::GetTarget() const
    {
        return this->target_;
    }
    void Camera::ClampPosition()
    {
        if (!this->limitRect_.has_value())
            return;
        if (!SDL_RectEmptyFloat(&this->limitRect_.value()))
        {
            if (this->viewRect_.x < this->limitRect_->x)
                this->viewRect_.x = this->limitRect_->x;
            if (this->viewRect_.y < this->limitRect_->y)
                this->viewRect_.y = this->limitRect_->y;
            if ((this->viewRect_.x + this->viewRect_.w > this->limitRect_->w) && (this->limitRect_->w > this->viewRect_.w))
                this->viewRect_.x = this->limitRect_->w - this->viewRect_.w;
            if ((this->viewRect_.y + this->viewRect_.h > this->limitRect_->h) && (this->limitRect_->h > this->viewRect_.h))
                this->viewRect_.y = this->limitRect_->y - this->viewRect_.y;
        }
    }
}
