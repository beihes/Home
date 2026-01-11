#include "../inc/PhysicsComponent.h"
#include "../inc/TransformComponent.h"
#include "object/inc/Object.h"
#include "physics/inc/PhysicsEngine.h"
#include "utils/log/inc/Log.h"

namespace engine::component {

    PhysicsComponent::PhysicsComponent(physics::PhysicsEngine* physicsEngine)
        : physicsEngine_(physicsEngine) {
        if (!this->physicsEngine_) {
            log::Error("[{}]PhysicsComponent构造函数中，PhysicsEngine指针不能为nullptr！", this->GetClassName());
        }
        b2BodyDef bodyDef = b2DefaultBodyDef();
        this->bodyId_ = this->physicsEngine_->CreateBody(bodyDef);
        if (B2_IS_NULL(this->bodyId_)) {
            log::Error("[{}]bodyId创建失败", this->GetClassName());
        }
        log::Trace("[{}]物理组件创建完成", this->GetClassName());
    }

    void PhysicsComponent::Init() {
        if (!this->owner_) {
            log::Error("[{}]物理组件初始化前需要一个GameObject作为所有者！", this->GetClassName());
            return;
        }
        if (!this->physicsEngine_) {
            log::Error("[{}]物理组件初始化时，PhysicsEngine未正确初始化。", this->GetClassName());
            return;
        }
        this->transform_ = this->owner_->GetComponent<TransformComponent>();
        if (!this->transform_) {
            log::Warn("[{}]物理组件初始化时，同一GameObject上没有找到TransformComponent组件。", this->GetClassName());
        }
        // 注册到PhysicsEngine
        this->physicsEngine_->RegisterComponent(this);
        log::Trace("[{}]物理组件初始化完成。", this->GetClassName());
    }

    void PhysicsComponent::Clean() {
        if (!B2_IS_NULL(bodyId_))
        {
            b2DestroyBody(bodyId_);
            bodyId_ = b2_nullBodyId;
        }
        if (this->physicsEngine_)
        {
            this->physicsEngine_->UnregisterComponent(this);
        }
        log::Trace("[{}]物理组件清理完成。", this->GetClassName());
    }

} // namespace engine::component