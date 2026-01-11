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

namespace engine::render {
    class Camera;
    class Renderer;
}

namespace engine::input {
    class InputManager;
}

namespace engine::object {
    class Object;
}

// namespace engine::ui {
//     class UIManager;
// }

namespace engine::scene {
    class SceneManager;
    /**
     * @brief 场景基类，负责管理场景中的游戏对象和场景生命周期。
     *
     * 包含一组游戏对象，并提供更新、渲染、处理输入和清理的接口。
     * 派生类应实现具体的场景逻辑。
     */
    class Scene {
    public:
        Scene() = default;
        /**
        * @brief 构造函数。
        *
        * @param name 场景的名称。
        * @param context 场景上下文。
        * @param scene_manager 场景管理器。
        */
        Scene(std::string name, core::Context& context, scene::SceneManager& sceneManager);
        virtual ~Scene();    // 1. 基类必须声明虚析构函数才能让派生类析构函数被正确调用。
        // 2. 析构函数定义必须写在cpp中，不然需要引入Object头文件
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(Scene&&) = delete;
    public:
        /// @brief 直接向场景中添加一个游戏对象。（初始化时可用，游戏进行中不安全） （&&表示右值引用，与std::move搭配使用，避免拷贝）
        virtual void AddObject(std::unique_ptr<object::Object>&& object);
        /// @brief 安全地添加游戏对象。（添加到pending_additions_中）
        virtual void SafeAddObject(std::unique_ptr<object::Object>&& object);
        /// @brief 直接从场景中移除一个游戏对象。（一般不使用，但保留实现的逻辑）
        virtual void RemoveObject(object::Object* objectPtr);
        /// @brief 安全地移除游戏对象。（设置need_remove_标记）
        virtual void SafeRemoveObject(object::Object* objectPtr);
        /// @brief 获取场景中的游戏对象容器。
        const std::vector<std::unique_ptr<object::Object>>& GetObjects() const { return this->gameObjects_; }
        /// @brief 根据名称查找游戏对象（返回找到的第一个对象）。
        object::Object* FindObjectByName(const std::string& name) const;
    public:
        // getters and setters
        void SetName(const std::string& name) { this->sceneName_ = name; }               ///< @brief 设置场景名称
        const std::string& GetName() const { return this->sceneName_; }                  ///< @brief 获取场景名称
        void SetInitialized(bool initialized) { this->isInitialized_ = initialized; }    ///< @brief 设置场景是否已初始化
        bool IsInitialized() const { return this->isInitialized_; }                      ///< @brief 获取场景是否已初始化
    public:
        core::Context& GetContext() const { return context_; }                  ///< @brief 获取上下文引用
        scene::SceneManager& GetSceneManager() const { return this->sceneManager_; } ///< @brief 获取场景管理器引用
        std::vector<std::unique_ptr<object::Object>>& GetObjects() { return this->gameObjects_; } ///< @brief 获取场景中的游戏对象
    protected:
        void ProcessPendingAdditions();     ///< @brief 处理待添加的游戏对象。（每轮更新的最后调用）
    public:
        // 核心循环方法
        virtual void Init();                        ///< @brief 初始化场景。
        virtual void Update(float deltaTime);      ///< @brief 更新场景。
        virtual void Render();                      ///< @brief 渲染场景。
        virtual void Input();                       ///< @brief 处理输入。
        virtual void Clean();                       ///< @brief 清理场景。
    protected:
        std::string sceneName_;                     ///< @brief 场景名称
        core::Context& context_;                    ///< @brief 上下文引用（隐式，构造时传入）
        scene::SceneManager& sceneManager_;         ///< @brief 场景管理器引用（构造时传入）
        // std::unique_ptr<ui::UIManager> uiManager_;  ///< @brief UI管理器(初始化时自动创建)

        bool isInitialized_ = false;                       ///< @brief 场景是否已初始化(非当前场景很可能未被删除，因此需要初始化标志避免重复初始化)
        std::vector<std::unique_ptr<object::Object>> gameObjects_;         ///< @brief 场景中的游戏对象
        std::vector<std::unique_ptr<object::Object>> pendingAdditions_;    ///< @brief 待添加的游戏对象（延时添加）
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}