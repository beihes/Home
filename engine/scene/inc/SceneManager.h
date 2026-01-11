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

namespace engine::core {
    class Context;
}

namespace engine::scene {
    class Scene;
    /**
     * @brief 场景基类，负责管理场景中的游戏对象和场景生命周期。
     *
     * 包含一组游戏对象，并提供更新、渲染、处理输入和清理的接口。
     * 派生类应实现具体的场景逻辑。
     */
    class SceneManager final {
    public:
        SceneManager() = default;
        /**
        * @brief 构造函数。
        *
        * @param name 场景的名称。
        * @param context 场景上下文。
        * @param scene_manager 场景管理器。
        */
        explicit SceneManager(core::Context& context);
        ~SceneManager();
    public:
        //删除拷贝和移动构造函数及赋值运算符
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;
    public:
        // 延时切换场景
        void RequestPushScene(std::unique_ptr<Scene>&& scene);      ///< @brief 请求压入一个新场景。
        void RequestPopScene();                                     ///< @brief 请求弹出当前场景。
        void RequestReplaceScene(std::unique_ptr<Scene>&& scene);   ///< @brief 请求替换当前场景。
        // getters
        Scene* GetCurrentScene() const;                                 ///< @brief 获取当前活动场景（栈顶场景）的指针。
        core::Context& GetContext() const { return this->context_; }  ///< @brief 获取引擎上下文引用。
    private:
        void ProcessPendingActions();                           ///< @brief 处理挂起的场景操作（每轮更新最后调用）。
        // 直接切换场景
        void PushScene(std::unique_ptr<Scene>&& scene);         ///< @brief 将一个新场景压入栈顶，使其成为活动场景。
        void PopScene();                                        ///< @brief 移除栈顶场景。
        void ReplaceScene(std::unique_ptr<Scene>&& scene);      ///< @brief 清理场景栈所有场景，将此场景设为栈顶场景。
    public:
        // 核心循环函数
        void Update(float deltaTime);
        void Render();
        void Input();
        void Close();
    private:
        core::Context& context_;                        ///< @brief 引擎上下文引用
        std::vector<std::unique_ptr<Scene>> sceneStack_;       ///< @brief 场景栈
        enum class PendingAction { None, Push, Pop, Replace };  ///< @brief 待处理的动作
        PendingAction pendingAction_ = PendingAction::None;    ///< @brief 待处理的动作
        std::unique_ptr<Scene> pendingScene_;                  ///< @brief 待处理场景
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}