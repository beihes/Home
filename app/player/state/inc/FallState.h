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

#include "PlayerState.h"

namespace app::player::state
{
    class FallState final : public PlayerState {
        friend class app::component::PlayerComponent;
    public:
        FallState(component::PlayerComponent* playerComponent) : PlayerState(playerComponent) {};
        ~FallState() override = default;
    private:
        void Enter() override;
        std::unique_ptr<PlayerState> Input(engine::core::Context& context) override;
        std::unique_ptr<PlayerState> Update(float deltaTime, engine::core::Context& context) override;
        void Exit() override;
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}