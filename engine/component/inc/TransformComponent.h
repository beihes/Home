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
#include <box2d/box2d.h>
#include <iostream>
#include <optional>

using namespace utils::rect;

namespace engine::core {
    class Context;
}

namespace engine::component {
    class TransformComponent final : public component::Component {
        friend class object::Object;//友元不能继承，必须每个子类单独添加
    public:
        TransformComponent() = default;
        TransformComponent(SDL_FPoint position = { 0,0 }, SDL_FPoint scale = { 1.f,1.f }, float rotation = 0.f) :position_(position), scale_(scale), rotation_(rotation) {}
        ~TransformComponent() override = default;
    public:
        //删除拷贝和移动构造函数及赋值运算符
        TransformComponent(const TransformComponent&) = delete;
        TransformComponent& operator=(const TransformComponent&) = delete;
        TransformComponent(TransformComponent&&) = delete;
        TransformComponent& operator=(TransformComponent&&) = delete;
    public:
        //获取位置（单位：米）
        b2Vec2 Get_Position_Meters()const { return b2Vec2{ this->P2M(this->position_.x),this->P2M(this->position_.y) }; }
        const SDL_FPoint& Get_Position_Pixels()const { return this->position_; }    //获取位置（单位：像素）
        const SDL_FPoint& Get_Scale()const { return this->scale_; }          //获取缩放
        float Get_Rotation()const { return this->rotation_; }                //获取角度
    public:
        //通过（单位：米）转换为像素位置
        void Set_Position(const b2Vec2& position) { this->position_ = SDL_FPoint{ this->M2P(position.x),this->M2P(position.y) }; }
        //设置（单位：像素）像素位置
        void Set_Position(const SDL_FPoint& position) { this->position_ = position; }
        void Set_Rotation(float rotation) { this->rotation_ = rotation; }
        void Set_Scale(const SDL_FPoint& scale);
        void Translate(const SDL_FPoint& offset) { this->position_ += offset; }//平移
    private:
        void UpdateSpriteSize();
    protected:
        //Component虚函数覆盖
        void Update(float, core::Context&)override {}
    private:
        SDL_FPoint position_ = { 0.f,0.f };//位置
        SDL_FPoint scale_ = { 1.f,1.f };//缩放
        float rotation_ = 0.f;  //角度，单位：度
    public:
        static constexpr float PPM = 100.0f;
        //米转为像素
        static constexpr float M2P(float m) noexcept { return m * PPM; }
        //像素转为米
        static constexpr float P2M(float p) noexcept { return p / PPM; }
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}
