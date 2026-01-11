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
#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "engine/component/inc/Component.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::core {
	class Context;
}

namespace engine::component {
	class Component;
}

namespace engine::object {
	class Object final {
	public:
		Object() = default;
		Object(const std::string& name = "", const std::string& tag = "");
		virtual ~Object() = default;// 虚函数确保正确清理派生类
	public:
		//删除拷贝和移动构造函数及赋值运算符
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&&) = delete;
		Object& operator=(Object&&) = delete;
		template <typename T,typename... Args>
		T* AddComponent(Args&&...args) {
			static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
			auto typeIndex = std::type_index(typeid(T));
			if (this->HasComponent<T>())
			{
				return this->GetComponent<T>();
			}
			//如果不存在		std::foreard -- 用于完美转发
			auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
			T* ptr = newComponent.get();
			newComponent->SetOwner(this);//设置组件拥有者
			this->components_[typeIndex] = std::move(newComponent);
			ptr->Init();
			log::Debug("[{}]{} added component {}", this->GetClassName(), this->name_.c_str(), typeid(T).name());
			return ptr;
		}
		template <typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
			auto typeIndex = std::type_index(typeid(T));
			auto it = this->components_.find(typeIndex);
			if (it == this->components_.end())
				return nullptr;
			return static_cast<T*>(it->second.get());
		}
		template <typename T>
		bool HasComponent()const {
			static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
			return this->components_.find(std::type_index(typeid(T))) != this->components_.end();
		}
		template <typename T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of<component::Component, T>::value, "T 必须继承自 Component");
			auto typeIndex = std::type_index(typeid(T));
			auto it = this->components_.find(typeIndex);
			if (it == this->components_.end())
				return;
			it->second->Clean();
			this->components_.erase(it);
		}
	public:
		void Update(float deltaTime, core::Context& context);
		void Input( core::Context& context);
		void Render( core::Context& context);
		void Clean();
	public:
		void SetName(const std::string& name) { this->name_ = name; }
		void SetTag(const std::string& tag) { this->tag_ = tag; }
		void SetNeedRemove(bool needRemove) { this->needRemove_ = needRemove; }
	public:
		const std::string& GetName()const { return this->name_; }
		const std::string& GetTag()const { return this->tag_; }
		bool IsNeedRemove()const { return this->needRemove_; }
	private:
		std::string name_;
		std::string tag_;
		std::unordered_map<std::type_index, std::unique_ptr<component::Component>> components_;
		bool needRemove_ = false;//延迟删除的标志，将来由场景类负责删除
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}
