#include "../inc/FallState.h"
#include "../inc/JumpState.h"
#include "../inc/IdleState.h"
#include "../inc/WalkState.h"
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
    void FallState::Enter() {
        log::Debug("[{}]进入FallState状态", this->GetClassName());
    }

    std::unique_ptr<PlayerState> FallState::Input(engine::core::Context& context) {
        auto inputManager = context.GetInputManager();
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        auto spriteComponent = this->playerComponent_->GetSpriteComponent();
        // 允许在跳跃时进行水平移动
        if (inputManager.IsActionDown("MoveLeft")) {
            if (physicsComponent->GetVelocity().x > 0.f) {
                physicsComponent->SetXVelocity(0.f);
            }
            physicsComponent->AddForce(b2Vec2{ -this->playerComponent_->GetMoveForce(), 0.0f }, false);
            spriteComponent->SetFlipped(true);
        }
        else if (inputManager.IsActionDown("MoveRight")) {
            if (physicsComponent->GetVelocity().x < 0.f) {
                physicsComponent->SetXVelocity(0.f);
            }
            physicsComponent->AddForce(b2Vec2{ this->playerComponent_->GetMoveForce(), 0.0f },false);
            spriteComponent->SetFlipped(false);
        }
        return nullptr; // 返回nullptr表示不切换状态
    }

    std::unique_ptr<PlayerState> FallState::Update(float deltaTime, engine::core::Context& context) {
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        //如果下方有碰撞，则切换到IdleState状态
        // if (physicsComponent->HasCollidedBelow()) {
        //     if (SDL_abs(physicsComponent->velocity_.x) < 1.f)
        //     {
        //         return std::make_unique<IdleState>(this->playerComponent_);
        //     } else {
        //         return std::make_unique<WalkState>(this->playerComponent_);
        //     }
        // }
        return nullptr; // 返回nullptr表示不切换状态
    }

    void FallState::Exit() {
        log::Debug("[{}]退出FallState状态", this->GetClassName());
    }
}