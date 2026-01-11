#include "../inc/TransformComponent.h"
#include "../inc/SpriteComponent.h"
#include "core/inc/Context.h"
#include "object/inc/Object.h"
#include "utils/log/inc/Log.h"

namespace engine::component {
    void TransformComponent::Set_Scale(const SDL_FPoint& scale)
    {
        this->scale_ = scale;
        if (this->owner_)
        {
            auto spriteComp = this->owner_->GetComponent<SpriteComponent>();
            if (spriteComp)
            {
                spriteComp->UpdateOffset();
            }
        }
    }
}