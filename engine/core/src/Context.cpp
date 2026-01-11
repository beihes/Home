#include "../inc/Context.h"
#include "input/inc/InputManager.h"
#include "physics/inc/PhysicsEngine.h"
#include "render/inc/Camera.h"
#include "render/inc/Renderer.h"
#include "render/inc/TextRenderer.h"
#include "resource/inc/ResourceManager.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::core
{
	Context::Context(render::Camera& camera, input::InputManager& inputManager, physics::PhysicsEngine& physicsEngine, render::Renderer& renderer,render::TextRenderer& textRenderer, resource::ResourceManager& resourceManager)
		:camera_(camera), inputManager_(inputManager), physicsEngine_(physicsEngine), renderer_(renderer), textRenderer_(textRenderer), resourceManager_(resourceManager)
	{
		log::Trace("[{}]上下文已创建并初始化，包括输入管理器、相机、渲染器和资源管理器", this->GetClassName());
	}
	Context::~Context() = default;
}
