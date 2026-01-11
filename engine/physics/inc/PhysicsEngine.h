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

// #include "utils/inc/SDL_rect_utlis.h"
#include <iostream>
#include <vector>
#include <utility>  // for std::pair
#include <optional>
#include <box2d/box2d.h>

namespace engine::component {
    class PhysicsComponent;
    class TileLayerComponent;
    enum class TileType;
}

namespace engine::object {
    class Object;
}

namespace engine::physics {

    /**
     * @brief 负责管理和模拟物理行为及碰撞检测。
     */
    class PhysicsEngine {
    public:
        PhysicsEngine();
        ~PhysicsEngine();
    public:
        // 禁止拷贝和移动
        PhysicsEngine(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(const PhysicsEngine&) = delete;
        PhysicsEngine(PhysicsEngine&&) = delete;
        PhysicsEngine& operator=(PhysicsEngine&&) = delete;
    public:
        void RegisterComponent(component::PhysicsComponent* component);     ///< @brief 注册物理组件
        void UnregisterComponent(component::PhysicsComponent* component);   ///< @brief 注销物理组件

        // 如果瓦片层需要进行碰撞检测则注册。（不需要则不必注册）
        void RegisterCollisionLayer(component::TileLayerComponent* layer);  ///< @brief 注册用于碰撞检测的 TileLayerComponent
        void UnregisterCollisionLayer(component::TileLayerComponent* layer);///< @brief 注销用于碰撞检测的 TileLayerComponent
    public:
        void Init();
        void Update(float deltaTime);      ///< @brief 核心循环：更新所有注册的物理组件的状态
        void Clear();
    public:
        void ProcessEvents();               /** @brief 处理本帧产生的碰撞事件 */
    public:
        b2BodyId CreateBody(b2BodyDef& bodyDef);
    public:
        // 设置器/获取器
        void SetGravity(const b2Vec2& gravity) { b2World_SetGravity(this->worldId_, gravity); }                                         ///< @brief 设置全局重力加速度
        void SetMaximumLinearSpeed(float maximumLinearSpeed) { b2World_SetMaximumLinearSpeed(this->worldId_, maximumLinearSpeed); }     ///< @brief 设置最大速度
        void SetWorldRect(SDL_FRect worldRect) { this->worldRect_ = worldRect; }                                                        ///< @brief 设置世界边界
    public:
        const b2Vec2& GetGravity() const { return b2World_GetGravity(this->worldId_); }                                                 ///< @brief 获取当前的全局重力加速度
        float GetMaximumLinearSpeed() const { return b2World_GetMaximumLinearSpeed(this->worldId_); }                                   ///< @brief 获取当前的最大速度
        const std::optional<SDL_FRect>& GetWorldRect() const { return this->worldRect_; }                                               ///< @brief 获取世界边界
        const b2WorldId& GetWorldId() const { return this->worldId_; }

    private:
        void ApplyWorldRect(component::PhysicsComponent* pc);     ///< @brief 应用世界边界，限制物体移动范围
    private:
        std::vector<component::PhysicsComponent*> components_;              ///< @brief 注册的物理组件容器，非拥有指针
        std::vector<component::TileLayerComponent*> collisionTileLayers_;   ///< @brief 注册的碰撞瓦片图层容器
        std::optional<SDL_FRect> worldRect_;    //世界边界矩形
        b2ChainId chainId_;         //空心箱体ID
        b2WorldId worldId_;         //世界ID
        int stepNum_ = 4;           //迭代次数
        b2ContactEvents contactEvents_;     //碰撞事件
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
} // namespace engine::physics