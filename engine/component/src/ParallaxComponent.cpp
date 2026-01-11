#include "../inc/ParallaxComponent.h"
#include "../inc/TransformComponent.h"
#include "core/inc/Context.h"
#include "render/inc/Camera.h"
#include "render/inc/Renderer.h"
#include "render/inc/Sprite.h"
#include "object/inc/Object.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::component {
    ParallaxComponent::ParallaxComponent(const std::string& textureId, SDL_FPoint scrollFactor, SDL_BPoint repeat)
        : sprite_(textureId), scrollFactor_(std::move(scrollFactor)), repeat_(std::move(repeat))
    {
        log::Trace("[{}]ParallaxComponent 初始化完成，纹理 ID: {}", this->GetClassName(), textureId);
    }

    void ParallaxComponent::Init() {
        if (!this->owner_) {
            log::Error("[{}]ParallaxComponent 初始化时，GameObject 为空。", this->GetClassName());
            return;
        }
        transform_ = owner_->GetComponent<TransformComponent>();
        if (!transform_) {
            log::Error("[{}]ParallaxComponent 初始化时，GameObject 上没有找到 TransformComponent 组件。", this->GetClassName());
            return;
        }
    }

    void ParallaxComponent::Render(core::Context& context) {
        if (this->isHidden_ || !transform_) { return; }
        // 直接调用视差滚动绘制函数
        context.GetRenderer().DrawParallax(context.GetCamera(), sprite_, transform_->Get_Position_Pixels(), this->scrollFactor_, this->repeat_, this->transform_->Get_Scale());
    }
}