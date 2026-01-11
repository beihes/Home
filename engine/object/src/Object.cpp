#include "../inc/Object.h"
#include "core/inc/Context.h"

namespace engine::object
{
	Object::Object(const std::string& name, const std::string& tag) :name_(name), tag_(tag)
	{
		log::Trace("[{}]Object created:{}{}", this->GetClassName(), this->name_, this->tag_);
	}
	void Object::Update(float deltaTime, core::Context& context)
	{
		for (auto& pair:this->components_)
		{
			pair.second->Update(deltaTime,context);
		}
	}
	void Object::Input( core::Context& context)
	{
		for (auto& pair : this->components_)
		{
			pair.second->Input(context);
		}
	}
	void Object::Render( core::Context& context)
	{
		for (auto& pair : this->components_)
		{
			pair.second->Render(context);
		}
	}
	void Object::Clean()
	{
		for (auto& pair : this->components_)
		{
			pair.second->Clean();
		}
	}
}
