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

#include <iostream>

namespace engine::core {
    class Context;
}

namespace engine::object {
    class Object;
}

namespace engine::component {
    class Component {
        friend class object::Object;
    public:
        Component() = default;
        virtual ~Component() = default;// 虚函数确保正确清理派生类
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;
    protected:
        virtual void Init() {};
    public:
        virtual void Input(core::Context& context) {};//处理输入
        virtual void Update(float deltaTime, core::Context& context) {};//更新
        virtual void Render(core::Context& context) {};//渲染
        virtual void Clean() {};//清理
    public:
        void SetOwner(object::Object* owner) { this->owner_ = owner; }
        object::Object* GetOwner()const { return this->owner_; }
    protected:
        object::Object* owner_ = nullptr;
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}
