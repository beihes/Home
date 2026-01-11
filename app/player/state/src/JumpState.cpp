#include "../inc/JumpState.h"
#include "../inc/IdleState.h"
#include "../inc/WalkState.h"
#include "../inc/FallState.h"
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
    void JumpState::Enter() {
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        //TODO: 目前是直接设置垂直速度，后期可以改进为根据当前速度和跳跃力计算
        physicsComponent->AddForce(b2Vec2{ 0,this->playerComponent_->GetJumpForce() }, false);
        log::Debug("[{}]进入JumpState状态，垂直方向初始速度设置为{}", this->GetClassName(), physicsComponent->GetVelocity().y);
    }

    std::unique_ptr<PlayerState> JumpState::Input(engine::core::Context& context) {
        auto inputManager = context.GetInputManager();
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        auto spriteComponent = this->playerComponent_->GetSpriteComponent();
        // 允许在跳跃时进行水平移动
        if (inputManager.IsActionDown("MoveLeft")) {
            physicsComponent->AddForce(b2Vec2{ -this->playerComponent_->GetMoveForce(), 0.0f },false);
            spriteComponent->SetFlipped(true);
        }
        else if (inputManager.IsActionDown("MoveRight")) {
            physicsComponent->AddForce(b2Vec2{ this->playerComponent_->GetMoveForce(), 0.0f },false);
            spriteComponent->SetFlipped(false);
        }
        return nullptr; // 返回nullptr表示不切换状态
    }

    std::unique_ptr<PlayerState> JumpState::Update(float deltaTime, engine::core::Context& context) {
        auto physicsComponent = this->playerComponent_->GetPhysicsComponent();
        //如果速度向下（y大于0），则切换到FallState状态
        // if (physicsComponent->velocity_.y > 0.f) {
        //     return std::make_unique<FallState>(this->playerComponent_);
        // }
        return nullptr; // 返回nullptr表示不切换状态
    }

    void JumpState::Exit() {
        log::Debug("[{}]退出JumpState状态", this->GetClassName());
    }
}