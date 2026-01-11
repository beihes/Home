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
#include <utility>
#include <box2d/box2d.h>

namespace engine::object {
	class Object;
}

namespace engine::physics {
	class PhysicsEngine;
}

namespace engine::component
{
    class ColliderComponent final : public Component {
        friend class object::Object;
    public:
        ColliderComponent() = default;
        ~ColliderComponent() override = default;
    public:
         // 删除复制/移动操作
        ColliderComponent(const ColliderComponent&) = delete;
        ColliderComponent& operator=(const ColliderComponent&) = delete;
        ColliderComponent(ColliderComponent&&) = delete;
        ColliderComponent& operator=(ColliderComponent&&) = delete;
    public:
        float GetDensity()const { return b2Shape_GetDensity(this->shapeId_); }
    public:
        void SetShapeId(b2ShapeId shapeId) { this->shapeId_ = shapeId; }
        void SetDensity(float density, bool updateBodyMass = true) { b2Shape_SetDensity(this->shapeId_, density, updateBodyMass); }          ///< @brief 设置质量，质量不能为负
    protected:
        // 核心循环方法
        void Init() override;
        void Update(float, core::Context&) override {};
        void Clean() override;
    private:
        b2ShapeId shapeId_;   //形状ID
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}