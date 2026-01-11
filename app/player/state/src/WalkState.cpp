#include "../inc/WalkState.h"
#include "../inc/FallState.h"
#include "../inc/IdleState.h"
#include "../inc/JumpState.h"
#include "engine/component/inc/PhysicsComponent.h"
#include "engine/component/inc/SpriteComponent.h"
#include "engine/component/inc/TransformComponent.h"
#include "engine/core/inc/Context.h"
#include "engine/input/inc/InputManager.h"
#include "app/component/inc/PlayerComponent.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace app::player::state
{
    void WalkState::Enter() {
        log::Debug("[{}]进入WalkState状态", this->GetClassName());
    }

    std::unique_ptr<PlayerState> WalkState::Input(engine::core::Context& context) {
        auto inputManager = context.GetInputManager();
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        auto spriteComponent = this->playerComponent_->GetSpriteComponent();
        if (inputManager.IsActionPressed("Jump"))
        {
            return std::make_unique<JumpState>(this->playerComponent_);
        }
        if (inputManager.IsActionDown("MoveLeft")) {
            physicsComponent->AddForce(b2Vec2{ -this->playerComponent_->GetMoveForce(), 0.0f },false);
            spriteComponent->SetFlipped(true);
        }
        else if (inputManager.IsActionDown("MoveRight")) {
            physicsComponent->AddForce(b2Vec2{ this->playerComponent_->GetMoveForce(), 0.0f },false);
            spriteComponent->SetFlipped(false);
        }
        else {
            // 没有按移动键，切换到Idle状态
            return std::make_unique<IdleState>(this->playerComponent_);
        }
        return nullptr; // 返回nullptr表示不切换状态
    }

    std::unique_ptr<PlayerState> WalkState::Update(float deltaTime, engine::core::Context& context) {
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        auto maxSpeed = this->playerComponent_->GetMaxSpeed();
        //如果下方没有碰撞，则切换到FallState状态
        // if (!physicsComponent->HasCollidedBelow()) {
            // return std::make_unique<FallState>(this->playerComponent_);
        // }
        return nullptr; // 返回nullptr表示不切换状态
    }

    void WalkState::Exit() {
        log::Debug("[{}]退出WalkState状态", this->GetClassName());
    }
} // namespace app::component::state