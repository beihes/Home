#include "../inc/AppScene.h"
#include "engine/component/inc/SpriteComponent.h"
#include "engine/component/inc/TileLayerComponent.h"
#include "engine/component/inc/TransformComponent.h"
#include "engine/component/inc/PhysicsComponent.h"
#include "engine/core/inc/Context.h"
#include "engine/input/inc/InputManager.h"
#include "engine/object/inc/Object.h"
#include "engine/physics/inc/PhysicsEngine.h"
#include "engine/render/inc/Camera.h"
#include "engine/render/inc/Renderer.h"
#include "engine/render/inc/TextRenderer.h"
#include "engine/scene/inc/SceneManager.h"
#include "engine/scene/inc/LevelLoader.h"
#include "component/inc/PlayerComponent.h"
#include "utils/log/inc/Log.h"
#include <entt/entt.hpp>
#include "web/inc/Customer.h"
#include "utils/net/inc/Net.h"

using namespace utils;

namespace app::scene {
    AppScene::AppScene(std::string name, engine::core::Context& context, engine::scene::SceneManager& sceneManager)
        :Scene(name, context, sceneManager) {
        log::Trace("[{}]GameScene构造完成", this->GetClassName());
    }

    void AppScene::Init() {
        // net::Test();
        // Customer midCustomer("2201150114", "296414");
        // midCustomer.SetSchedule("25-26-1");
        // midCustomer.SetData();
        if (!this->InitLevel())
        {
            log::Error("[{}]场景初始化失败：关卡加载失败", this->GetClassName());
            return;
        }
        if (!this->InitPlayer())
        {
            log::Error("[{}]场景初始化失败：玩家初始化失败", this->GetClassName());
            return;
        }
        Scene::Init();
        log::Trace("[{}]GameScene初始化完成", this->GetClassName());
    }
    void AppScene::Update(float deltaTime) {
        Scene::Update(deltaTime);
    }

    void AppScene::Render() {
        Scene::Render();
        this->Test_TextRenderer();
    }

    void AppScene::Input() {
        Scene::Input();
    }

    void AppScene::Clean() {
        auto& inputManager = this->context_.GetInputManager();
        inputManager.OnAction("Attack").disconnect<&AppScene::OnAttack>(this);
        inputManager.OnAction("Jump", engine::input::ActionState::RELEASED).disconnect<&AppScene::OnJump>(this);
        Scene::Clean();
    }

    bool AppScene::InitLevel() {
        engine::scene::LevelLoader levelLoader;
        if (!levelLoader.LoadLevel("assets/maps/level1.tmj", *this))
        {
            log::Error("[{}]关卡 {} 加载失败", this->GetClassName(), levelLoader.mapPath_);
            return false;
        }
        // 注册"main"层到物理引擎
        auto* mainLayer = this->FindObjectByName("main");
        if (!mainLayer)
        {
            log::Error("[{}]未找到\"main\"层", this->GetClassName());
            return false;
        }
        auto* tileLayer = mainLayer->GetComponent<engine::component::TileLayerComponent>();
        if (!tileLayer) {
            log::Error("[{}]\"main\"层缺少 TileLayerComponent 组件", this->GetClassName());
            return false;
        }
        this->context_.GetPhysicsEngine().RegisterCollisionLayer(tileLayer);

        log::Info("[{}]注册\"main\"层到物理引擎", this->GetClassName());
        auto worldSize = mainLayer->GetComponent<engine::component::TileLayerComponent>()->GetWorldSize();
        this->context_.GetCamera().SetLimitRect(SDL_FRect{ 0.0f, 0.0f, float(worldSize.x), float(worldSize.y) });
        this->context_.GetPhysicsEngine().SetWorldRect(SDL_FRect{ 0.0f, 0.0f, float(worldSize.x), float(worldSize.y) });
        log::Info("[{}]关卡初始化完成", this->GetClassName());
        return true;
    }

    bool AppScene::InitPlayer() {
        // 获取玩家对象
        this->player_ = this->FindObjectByName("player");
        if (!player_) {
            log::Error("[{}]未找到玩家对象", this->GetClassName());
            return false;
        }
        auto* playerComponent = this->player_->AddComponent<app::component::PlayerComponent>();
        if (!playerComponent) {
            log::Error("[{}]无法添加 PlayerComponent 到玩家对象", this->GetClassName());
            return false;
        }
        auto* playerTransform = playerComponent->GetTransformComponent();
        if (!playerTransform) {
            log::Error("[{}]玩家对象缺少 TransformComponent 组件", this->GetClassName());
            return false;
        }
        this->context_.GetCamera().SetTarget(playerTransform);
        auto& inputManager = this->context_.GetInputManager();
        inputManager.OnAction("Attack").connect<&AppScene::OnAttack>(this);
        inputManager.OnAction("Jump", engine::input::ActionState::RELEASED).connect<&AppScene::OnJump>(this);
        log::Trace("[{}]Player初始化完成", this->GetClassName());
        return true;
    }

    void AppScene::OnAttack() {
        log::Info("[{}]玩家发动攻击", this->GetClassName());
    }

    void AppScene::OnJump() {
        log::Info("[{}]玩家跳跃", this->GetClassName());
    }

    void AppScene::Test_TextRenderer() {
        auto& textRenderer = this->context_.GetTextRenderer();
        const auto& camera = this->context_.GetCamera();
        textRenderer.DrawUIText("《》北河三", "assets/fonts/VonwaonBitmap-16px.ttf", 16, { 100.0f, 100.0f }, { 0.f, 1.0f, 0.f, 1.0f });
        textRenderer.DrawText(camera, "Map Text", "assets/fonts/SourceHanSansSC-Normal.otf", 16, { 200.0f, 200.0f });
    }
}
