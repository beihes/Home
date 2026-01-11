#include "../inc/IdleState.h"
#include "../inc/WalkState.h"
#include "../inc/FallState.h"
#include "../inc/JumpState.h"
#include "engine/component/inc/PhysicsComponent.h"
#include "engine/component/inc/SpriteComponent.h"
#include "engine/component/inc/TransformComponent.h"
#include "engine/core/inc/Context.h"
#include "engine/input/inc/InputManager.h"
#include "component/inc/PlayerComponent.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace app::player::state
{
    void IdleState::Enter() {
        log::Debug("[{}]进入IdleState状态", this->GetClassName());
    }

    std::unique_ptr<PlayerState> IdleState::Input(engine::core::Context& context) {
        auto inputManager = context.GetInputManager();
        if (inputManager.IsActionPressed("Jump")) {
            return std::make_unique<JumpState>(this->playerComponent_);
        }
        if (inputManager.IsActionDown("MoveLeft") || inputManager.IsActionDown("MoveRight")) {
            return std::make_unique<WalkState>(this->playerComponent_);
        }
        return nullptr; // 返回nullptr表示不切换状态
    }

    std::unique_ptr<PlayerState> IdleState::Update(float deltaTime, engine::core::Context& context) {
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        auto frictionFactor = this->playerComponent_->GetFrictionFactor();
        // physicsComponent->velocity_.x *= frictionFactor;
        //如果下方没有碰撞，则切换到FallState状态
        // if (!physicsComponent->HasCollidedBelow()) {
        //     return std::make_unique<FallState>(this->playerComponent_);
        // }
        return nullptr; // 返回nullptr表示不切换状态
    }

    void IdleState::Exit() {
        log::Debug("[{}]退出IdleState状态", this->GetClassName());
    }
}