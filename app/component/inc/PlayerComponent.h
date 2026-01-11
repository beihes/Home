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

#include "engine/component/inc/Component.h"
#include "player/state/inc/PlayerState.h"
#include <memory>

namespace engine::input {
    class InputManager;
}

namespace engine::component {
    class TransformComponent;
    class PhysicsComponent;
    class SpriteComponent;
}

namespace app::component
{
    class PlayerComponent final : public engine::component::Component {
        friend class engine::object::Object;
    public:
        PlayerComponent() = default;
        ~PlayerComponent() override = default;
    public:
    public:
        //删除拷贝和移动构造函数及赋值运算符
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) = delete;
    public:
        void SetCurrentState(std::unique_ptr<player::state::PlayerState> newState);
        void SetIsDead(bool isDead) { this->isDead_ = isDead; }
        void SetMoveForce(float force) { this->moveForce_ = force; }
        void SetJumpForce(float force) { this->jumpForce_ = force; }
        void SetFrictionFactor(float factor) { this->frictionFactor_ = factor; }
        void SetMaxSpeed(float speed) { this->maxSpeed_ = speed; }
    public:
        engine::component::PhysicsComponent* GetPhysicsComponent() const { return this->physicsComponent_; }
        engine::component::SpriteComponent* GetSpriteComponent() const { return this->spriteComponent_; }
        engine::component::TransformComponent* GetTransformComponent() const { return this->transformComponent_; }
        bool GetIsDead() const { return this->isDead_; }
        float GetMoveForce() const { return this->moveForce_; }
        float GetJumpForce() const { return this->jumpForce_; }
        float GetFrictionFactor() const { return this->frictionFactor_; }
        float GetMaxSpeed() const { return this->maxSpeed_; }
    private:
        void Init() override;
        void Input(engine::core::Context& context) override;
        void Update(float deltaTime, engine::core::Context& context) override;
        // void Clean() override;
    private:
        engine::component::PhysicsComponent* physicsComponent_ = nullptr;
        engine::component::SpriteComponent* spriteComponent_ = nullptr;
        engine::component::TransformComponent* transformComponent_ = nullptr;
        std::unique_ptr<player::state::PlayerState> currentState_ = nullptr;
        bool isDead_ = false;
        float moveForce_ = 200.0f; ///< 水平移动时施加的力
        float jumpForce_ = 200.0f; ///< 跳跃时施加的力
        float frictionFactor_ = 0.85f; ///< 摩擦系数，控制水平减速（Idle时缓冲效果，每帧乘以此系数）
        float maxSpeed_ = 120.0f;   ///< 最大水平速度
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace app::component