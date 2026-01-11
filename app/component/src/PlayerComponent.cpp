#include "../inc/PlayerComponent.h"
#include "engine/component/inc/PhysicsComponent.h"
#include "engine/component/inc/SpriteComponent.h"
#include "engine/component/inc/TransformComponent.h"
#include "engine/core/inc/Context.h"
#include "engine/object/inc/Object.h"
#include "player/state/inc/IdleState.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace app::component {
    void PlayerComponent::SetCurrentState(std::unique_ptr<player::state::PlayerState> newState) {
        if (!newState)
        {
            log::Error("[{}]尝试设置空状态指针", this->GetClassName());
            return;
        }
        if (this->currentState_) {
            this->currentState_->Exit();
        }
        this->currentState_ = std::move(newState);
        log::Debug("[{}]切换到新状态：{}", this->GetClassName(), typeid(*this->currentState_).name());
        this->currentState_->Enter();
    }
    void PlayerComponent::Init() {
        if (!this->owner_) {
            log::Error("[{}]PlayerComponent没有所属对象指针", this->GetClassName());
            return;
        }
        this->physicsComponent_ = this->owner_->GetComponent<engine::component::PhysicsComponent>();
        this->spriteComponent_ = this->owner_->GetComponent<engine::component::SpriteComponent>();
        this->transformComponent_ = this->owner_->GetComponent<engine::component::TransformComponent>();
        if (!this->physicsComponent_ || !this->spriteComponent_ || !this->transformComponent_)
        {
            log::Error("[{}]PlayerComponent缺少必要的组件", this->GetClassName());
        }
        this->currentState_ = std::make_unique<player::state::IdleState>(this);
        if (!this->currentState_)
        {
            log::Error("[{}]PlayerComponent无法创建初始状态", this->GetClassName());
        } else {
            this->SetCurrentState(std::move(this->currentState_));
        }
        log::Debug("[{}]PlayerComponent初始化完成", this->GetClassName());
    }
    void PlayerComponent::Input(engine::core::Context& context) {
        if (!this->currentState_)return;
        auto nextState = this->currentState_->Input(context);
        if (nextState) {
            this->SetCurrentState(std::move(nextState));
        }
    }
    void PlayerComponent::Update(float deltaTime, engine::core::Context& context) {
         if (!this->currentState_)return;
        auto nextState = this->currentState_->Update(deltaTime, context);
        if (nextState) {
            this->SetCurrentState(std::move(nextState));
        }
    }
}