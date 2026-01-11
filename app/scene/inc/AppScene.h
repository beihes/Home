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
#include <vector>
#include <memory>

#include "engine/scene/inc/Scene.h"

namespace engine::core {
    class Context;
}

namespace engine::object {
    class Object;
}

// namespace engine::ui {
//     class UIManager;
// }


namespace app::scene {
    class AppScene final : public engine::scene::Scene {
    public:
        AppScene(std::string name, engine::core::Context& context, engine::scene::SceneManager& sceneManager);
    public:
        // 核心循环方法
        void Init()override;                        ///< @brief 初始化场景。
        void Update(float deltaTime)override;      ///< @brief 更新场景。
        void Render()override;                      ///< @brief 渲染场景。
        void Input()override;                       ///< @brief 处理输入。
        void Clean()override;                       ///< @brief 清理场景。
    private:
        bool InitLevel();
        bool InitPlayer();
        void OnAttack();
        void OnJump();
        void Test_TextRenderer();
    public:
        engine::object::Object* player_ = nullptr;  ///< @brief 保存玩家对象的指针，方便访问
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}