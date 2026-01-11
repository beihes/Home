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

#include <memory>

namespace engine::core {
    class Context;
}

namespace app::component {
    class PlayerComponent;
}

namespace app::player::state {
    class PlayerState {
        friend class app::component::PlayerComponent;
    public:
        PlayerState(component::PlayerComponent* playerComponent) :playerComponent_(playerComponent) {}
        virtual ~PlayerState() = default;
    public:
        //删除拷贝和移动构造函数及赋值运算符
        PlayerState(const PlayerState&) = delete;
        PlayerState& operator=(const PlayerState&) = delete;
        PlayerState(PlayerState&&) = delete;
        PlayerState& operator=(PlayerState&&) = delete;
    public:
        virtual void Enter() = 0;                             ///< @brief 进入状态时调用
        virtual std::unique_ptr<PlayerState> Input(engine::core::Context& context) = 0;  ///< @brief 处理输入
        virtual std::unique_ptr<PlayerState> Update(float deltaTime, engine::core::Context& context) = 0; ///< @brief 更新状态
        virtual void Exit() = 0;                              ///< @brief 退出状态时调用
    protected:
        component::PlayerComponent* playerComponent_ = nullptr;
    };
}