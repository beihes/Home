#include "../inc/PhysicsEngine.h"
#include "component/inc/PhysicsComponent.h"
#include "component/inc/TransformComponent.h"
#include "component/inc/TileLayerComponent.h"
#include "object/inc/Object.h"
#include "utils/rect/inc/Rect.h"
#include "utils/log/inc/Log.h"
#include <set>

using namespace utils;
using namespace utils::rect;

namespace engine::physics {

    PhysicsEngine::PhysicsEngine()
        : worldId_(b2_nullWorldId) {
        this->Init();
    }

    PhysicsEngine::~PhysicsEngine() {
        this->Clear();
    }

    void PhysicsEngine::RegisterComponent(component::PhysicsComponent* component) {
        this->components_.push_back(component);
        log::Trace("[{}]物理组件注册完成。", this->GetClassName());
    }

    void PhysicsEngine::UnregisterComponent(component::PhysicsComponent* component) {
        // 使用 remove-erase 方法安全地移除指针
        auto it = std::remove(this->components_.begin(), this->components_.end(), component);
        this->components_.erase(it, this->components_.end());
        log::Trace("[{}]物理组件注销完成。", this->GetClassName());
    }

    void PhysicsEngine::RegisterCollisionLayer(component::TileLayerComponent* layer) {
        layer->SetPhysicsEngine(this); // 设置物理引擎指针
        this->collisionTileLayers_.push_back(layer);
        log::Trace("[{}]碰撞瓦片图层注册完成。", this->GetClassName());
    }

    void PhysicsEngine::UnregisterCollisionLayer(component::TileLayerComponent* layer) {
        auto it = std::remove(this->collisionTileLayers_.begin(), this->collisionTileLayers_.end(), layer);
        this->collisionTileLayers_.erase(it, this->collisionTileLayers_.end());
        log::Trace("[{}]碰撞瓦片图层注销完成。", this->GetClassName());
    }

    void PhysicsEngine::Init() {
        if (!B2_IS_NULL(this->worldId_)) {
            log::Warn("[{}]物理世界已存在，重复 Init 被忽略", GetClassName());
            return;
        }
        b2Version ver = b2GetVersion();
        log::Info("Box2D version: {}.{}.{}", ver.major, ver.minor, ver.revision);
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = { 0.0f, -9.8f };
        this->worldId_ = b2CreateWorld(&worldDef);
        if (B2_IS_NULL(this->worldId_)) {
            log::Error("[{}]物理引擎初始化失败", this->GetClassName());
            return;
        }
        log::Trace("[{}]物理引擎初始完成", this->GetClassName());
    }

    void PhysicsEngine::Update(float deltaTime) {
        // 推进物理世界
        // 参数：世界、时间步长、速度迭代次数
        if (B2_IS_NULL(this->worldId_))return;
        b2World_Step(this->worldId_, deltaTime, this->stepNum_);
        // log::Debug("[{}]bodyId num:{}", this->GetClassName(), b2World_GetCounters(this->worldId_).bodyCount);
    }

    void PhysicsEngine::Clear() {
        if (!B2_IS_NULL(this->worldId_)) {
            b2DestroyWorld(this->worldId_);
            this->worldId_ = b2_nullWorldId;
            log::Trace("[{}]物理引擎已销毁", GetClassName());
        }
    }

    void PhysicsEngine::ProcessEvents() {

    }

    b2BodyId PhysicsEngine::CreateBody(b2BodyDef& bodyDef) {
        return b2CreateBody(this->worldId_, &bodyDef);
    }

    void PhysicsEngine::ApplyWorldRect(component::PhysicsComponent* pc) {
        if (!pc || !this->worldRect_) return;

    }
} // namespace engine::physics 
