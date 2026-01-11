#include "../inc/Scene.h"
#include "../inc/SceneManager.h"
#include "object/inc/Object.h"
#include "core/inc/Context.h"
#include "physics/inc/PhysicsEngine.h"
#include "render/inc/Camera.h"
// #include "../../ui/inc/UIManager.h"
#include "utils/log/inc/Log.h"
#include <algorithm> // for std::remove_if

using namespace utils;

namespace engine::scene {

    Scene::Scene(std::string name, engine::core::Context& context, engine::scene::SceneManager& sceneManager)
        : sceneName_(name), context_(context), sceneManager_(sceneManager), isInitialized_(false) {
        log::Trace("[{}]场景 '{}' 构造完成。", this->GetClassName(), this->sceneName_);
    }

    Scene::~Scene() = default;

    void Scene::Init() {
        this->isInitialized_ = true;     // 子类应该最后调用父类的 init 方法
        log::Trace("[{}]场景 '{}' 初始化完成。", this->GetClassName(), this->sceneName_);
    }

    void Scene::Update(float deltaTime) {
        if (!this->isInitialized_) return;
        this->context_.GetPhysicsEngine().Update(deltaTime);
        this->context_.GetCamera().Update(deltaTime);
        bool needRemove = false;  // 设定一个标志，用于判断是否需要移除对象
        // 更新所有游戏对象，先略过需要移除的对象
        for (auto& obj : this->gameObjects_) {
            if (obj && !obj->IsNeedRemove()) {
                obj->Update(deltaTime, context_);
            } else {
                needRemove = true;
                if (obj) obj->Clean();  // 如果对象需要移除，则先调用clean方法
                else log::Warn("[{}]尝试更新一个空的游戏对象指针。", this->GetClassName());
            }
        }
        if (needRemove) {
            this->gameObjects_.erase(
                std::remove_if(
                    this->gameObjects_.begin(),
                    this->gameObjects_.end(),
                    [](const std::unique_ptr<object::Object>& obj) {return !obj || obj->IsNeedRemove();}
                ),
                this->gameObjects_.end()
            );
        }
        this->ProcessPendingAdditions();      // 处理待添加（延时添加）的游戏对象
    }

void Scene::Render() {
     if (!this->isInitialized_) return;
    // 渲染所有游戏对象
    for (const auto& obj : this->gameObjects_) {
        if (obj) obj->Render(context_);
    }
    // 渲染UI管理器
    // ui_manager_->render(context_);
}

void Scene::Input() {
     if (!this->isInitialized_) return;
    // 处理UI管理器输入
    // if (ui_manager_->handleInput(context_)) return;   // 如果输入事件被UI处理则返回，不再处理游戏对象输入
    // 遍历所有游戏对象，略过需要移除的对象
     for (auto& obj : this->gameObjects_) {
         if (obj && !obj->IsNeedRemove()) { obj->Input(context_); }
     }
    // 不在这里移除，以免浪费算力。在update中移除
}

void Scene::Clean() {
    if (!this->isInitialized_) return;
    for (const auto& obj : this->gameObjects_) {
        if (obj) obj->Clean();
    }
    this->gameObjects_.clear();
    this->isInitialized_ = false;        // 清理完成后，设置场景为未初始化
    this->context_.GetPhysicsEngine().Clear();
    log::Trace("[{}]场景 '{}' 清理完成。", this->GetClassName(), this->sceneName_);
}

void Scene::AddObject(std::unique_ptr<object::Object>&& object) {
    if (object) this->gameObjects_.push_back(std::move(object));
    else log::Warn("[{}]尝试向场景 '{}' 添加空游戏对象。", this->GetClassName(), this->sceneName_);
}

void Scene::SafeAddObject(std::unique_ptr<object::Object>&& object)
{
    if (object) this->pendingAdditions_.push_back(std::move(object));
    else log::Warn("[{}]尝试向场景 '{}' 添加空游戏对象。", this->GetClassName(), this->sceneName_);
}

void Scene::RemoveObject(object::Object* objectPtr) {
    if (!objectPtr) {
        log::Warn("[{}]尝试从场景 '{}' 中移除一个空的游戏对象指针。", this->GetClassName(), this->sceneName_);
        return;
    }
    // erase-remove 移除法不可用，因为智能指针与裸指针无法比较
    // 需要使用 std::remove_if 和 lambda 表达式自定义比较的方式
    auto it = std::remove_if(this->gameObjects_.begin(), this->gameObjects_.end(),
                             [objectPtr](const std::unique_ptr<object::Object>& p) {
                                 return p.get() == objectPtr;    // 比较裸指针是否相等（自定义比较方式）
                             });

    if (it != this->gameObjects_.end()) {
        (*it)->Clean();             // 因为传入的是指针，因此只可能有一个元素被移除，不需要遍历it到末尾
        this->gameObjects_.erase(it, this->gameObjects_.end());   // 删除从it到末尾的元素（最后一个元素）
        log::Trace("[{}]从场景 '{}' 中移除游戏对象。", this->GetClassName(), this->sceneName_);
    } else {
        log::Warn("[{}]游戏对象指针未找到在场景 '{}' 中。", this->GetClassName(), this->sceneName_);
    }
}

void Scene::SafeRemoveObject(object::Object* objectPtr)
{
    objectPtr->SetNeedRemove(true);
}

object::Object* Scene::FindObjectByName(const std::string& name) const
{
    // 找到第一个符合条件的游戏对象就返回
    for (const auto& obj : this->gameObjects_) {
        if (obj && obj->GetName() == name) { return obj.get(); }
    }
    return nullptr;
}

void Scene::ProcessPendingAdditions()
{
    // 处理待添加的游戏对象
    for (auto& game_object : this->pendingAdditions_) {
        this->AddObject(std::move(game_object));
    }
    this->pendingAdditions_.clear();
}

} // namespace engine::scene 