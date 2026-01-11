#include "../inc/SceneManager.h"
#include "../inc/Scene.h"
#include "core/inc/Context.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::scene {

    SceneManager::SceneManager(engine::core::Context& context)
        : context_(context) {
        log::Trace("[{}]场景管理器已创建。", this->GetClassName());
    }

    SceneManager::~SceneManager() {
        log::Trace("[{}]场景管理器已销毁。", this->GetClassName());
        this->Close(); // 即使不手动调用 close 也能确保清理
    }

    Scene* SceneManager::GetCurrentScene() const {
        if (this->sceneStack_.empty()) { return nullptr; }
        return this->sceneStack_.back().get(); // 返回栈顶场景的裸指针
    }

    void SceneManager::RequestPopScene()
    {
        this->pendingAction_ = PendingAction::Pop;
    }

    void SceneManager::RequestReplaceScene(std::unique_ptr<Scene>&& scene)
    {
        this->pendingAction_ = PendingAction::Replace;
        this->pendingScene_ = std::move(scene);
    }

    void SceneManager::RequestPushScene(std::unique_ptr<Scene>&& scene)
    {
        this->pendingAction_ = PendingAction::Push;
        this->pendingScene_ = std::move(scene);
    }

    // --- Private Methods ---

    void SceneManager::ProcessPendingActions()
    {
        if (this->pendingAction_ == PendingAction::None) {
            return;
        }
        switch (this->pendingAction_) {
        case PendingAction::Pop:        this->PopScene();break;
        case PendingAction::Replace:    this->ReplaceScene(std::move(pendingScene_));break;
        case PendingAction::Push:       this->PushScene(std::move(pendingScene_));break;
        default:                        break;
        }
        this->pendingAction_ = PendingAction::None;
    }

    void SceneManager::PushScene(std::unique_ptr<Scene>&& scene) {
        if (!scene) {
            log::Warn("[{}]尝试将空场景压入栈。", this->GetClassName());
            return;
        }
        log::Debug("[{}]正在将场景 '{}' 压入栈。", this->GetClassName(), scene->GetName());
        // 初始化新场景
        if (!scene->IsInitialized()) { scene->Init(); }// 确保只初始化一次
        // 将新场景移入栈顶
        this->sceneStack_.push_back(std::move(scene));
    }

    void SceneManager::PopScene() {
        if (this->sceneStack_.empty()) {
            log::Warn("[{}]尝试从空场景栈中弹出", this->GetClassName());
            return;
        }
        log::Debug("[{}]正在从栈中弹出场景 '{}'", this->GetClassName(), this->sceneStack_.back()->GetName());
        // 清理并移除栈顶场景
        if (this->sceneStack_.back()) { this->sceneStack_.back()->Clean(); }// 显式调用清理
        this->sceneStack_.pop_back();
    }

    void SceneManager::ReplaceScene(std::unique_ptr<Scene>&& scene) {
        if (!scene) {
            log::Warn("[{}]尝试用空场景替换。", this->GetClassName());
            return;
        }
        log::Debug("[{}]正在用场景 '{}' 替换场景 '{}' 。", this->GetClassName(), scene->GetName(), this->sceneStack_.back()->GetName());
        // 清理并移除场景栈中所有场景
        while (!this->sceneStack_.empty()) {
            if (this->sceneStack_.back()) { this->sceneStack_.back()->Clean(); }
            this->sceneStack_.pop_back();
        }
        // 初始化新场景
        if (!scene->IsInitialized()) { scene->Init(); }
        // 将新场景压入栈顶
        this->sceneStack_.push_back(std::move(scene));
    }

    void SceneManager::Update(float deltaTime) {
        // 只更新栈顶（当前）场景
        Scene* currentScene = this->GetCurrentScene();
        if (currentScene) { currentScene->Update(deltaTime); }
        // 执行可能的切换场景操作
        this->ProcessPendingActions();
    }

    void SceneManager::Render() {
        // 渲染时需要叠加渲染所有场景，而不只是栈顶
        for (const auto& scene : this->sceneStack_) {
            if (scene) { scene->Render(); }
        }
    }

    void SceneManager::Input() {
        // 只考虑栈顶场景
        Scene* currentScene = this->GetCurrentScene();
        if (currentScene) { currentScene->Input(); }
    }

    void SceneManager::Close() {
        log::Trace("[{}]正在关闭场景管理器并清理场景栈...", this->GetClassName());
        // 清理栈中所有剩余的场景（从顶到底）
        while (!this->sceneStack_.empty()) {
            if (this->sceneStack_.back()) {
                log::Debug("[{}]正在清理场景 '{}' 。", this->GetClassName(), this->sceneStack_.back()->GetName());
                this->sceneStack_.back()->Clean();
            }
            this->sceneStack_.pop_back();
        }
    }

} // namespace engine::scene