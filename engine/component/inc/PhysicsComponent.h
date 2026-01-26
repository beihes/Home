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

#include "Component.h"
#include "utils/rect/inc/Rect.h"
#include <utility>
#include <box2d/box2d.h>

namespace engine::object {
    class Object;
}

namespace engine::physics {
    class PhysicsEngine;
}

// using namespace utils::rect;

namespace engine::component {
    class TransformComponent;

    /**
     * @brief 管理GameObject的物理属性
     *
     * 存储速度、质量、力和重力设置。与PhysicsEngine交互。
     */
    class PhysicsComponent final : public Component {
        friend class object::Object;
        friend class physics::PhysicsEngine;
    public:
        /**
         * @brief 构造函数
         *
         * @param physicsEngine 指向PhysicsEngine的指针，不能为nullptr
         * @param useGravity 物体是否受重力影响，默认true
         * @param mass 物体质量，默认1.0
         */
        PhysicsComponent(physics::PhysicsEngine* physicsEngine);
        ~PhysicsComponent() override = default;
    public:
        // 删除复制/移动操作
        PhysicsComponent(const PhysicsComponent&) = delete;
        PhysicsComponent& operator=(const PhysicsComponent&) = delete;
        PhysicsComponent(PhysicsComponent&&) = delete;
        PhysicsComponent& operator=(PhysicsComponent&&) = delete;
    public:
        // PhysicsEngine使用的物理方法
        void AddForce(const b2Vec2& force, bool wake = true) { b2Body_ApplyForceToCenter(this->bodyId_, force, wake); }                     ///< @brief 在质心添加力
        void AddForce(const b2Vec2& force, const b2Vec2& pos, bool wake = true) { b2Body_ApplyForce(this->bodyId_, force, pos, wake); }     ///< @brief 在自定义点添加力

        ///< @brief 如果不行调用就把当前力注销掉，不加进去，清除力太麻烦了
        // void ClearForce() { b2Body_SetLinearVelocity(this->bodyId_, { 0,0 })}

        // const b2Vec2& GetForce() const { return this->a; }                       ///< @brief 获取当前力
        float GetMass() const { return b2Body_GetMass(this->bodyId_); }             ///< @brief 获取质量
        bool IsAwake() const { return b2Body_IsAwake(this->bodyId_); }                                 ///< @brief 获取组件是否启用
        bool IsUseGravity() const { return b2Body_GetGravityScale(this->bodyId_) != 0.0f; }                          ///< @brief 获取组件是否受重力影响
        b2Vec2 GetVelocity() const { return b2Body_GetLinearVelocity(this->bodyId_); }         ///< @brief 获取当前速度
        TransformComponent* GetTransform() const { return this->transform_; }       ///< @brief 获取TransformComponent指针
        b2Vec2 GetPosition()const { return b2Body_GetPosition(this->bodyId_); }
        b2Rot GetRotation()const { return b2Body_GetRotation(this->bodyId_); }
    public:
        // 设置器/获取器
        void SetAwake(bool awake) { b2Body_SetAwake(this->bodyId_, awake); }                       ///< @brief 设置组件是否启用
        void SetTransform(const b2Vec2& pos) { b2Body_SetTransform(this->bodyId_, pos, b2Body_GetRotation(this->bodyId_)); }    ///< @brief 将Body移动到pos
        void SetUseGravity(bool useGravity) { b2Body_SetGravityScale(this->bodyId_, useGravity ? 1.0f : 0.0f); }        ///< @brief 设置组件是否受重力影响
        void SetXVelocity(float xVelocity) { b2Body_SetLinearVelocity(this->bodyId_, b2Vec2{xVelocity, b2Body_GetLinearVelocity(this->bodyId_).y}); }
        void SetYVelocity(float yVelocity) { b2Body_SetLinearVelocity(this->bodyId_, b2Vec2{b2Body_GetLinearVelocity(this->bodyId_).x, yVelocity}); }
        void SetVelocity(const b2Vec2& velocity) { b2Body_SetLinearVelocity(this->bodyId_, velocity);; }       ///< @brief 设置速度
    private:
        void SetBodyId(b2BodyId bodyId) { this->bodyId_ = bodyId; }
    protected:
        // 核心循环方法
        void Init() override;
        void Update(float, core::Context&) override {};
        void Clean() override;
    private:
        b2BodyId bodyId_;               //物理ID
    private:
        physics::PhysicsEngine* physicsEngine_ = nullptr;   ///< @brief 指向PhysicsEngine的指针
        TransformComponent* transform_ = nullptr;           ///< @brief TransformComponent的缓存指针
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::component