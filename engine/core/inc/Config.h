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
#include <nlohmann/json.hpp>

namespace engine::core {
	class Config final {
	public:
		Config() = default;
		explicit Config(const std::string& fileDirectory);//构造函数，指定配置文件路径
		~Config() = default;
	public:
		//删除拷贝和移动构造函数及赋值运算符
		Config(const Config&) = delete;
		Config& operator=(const Config&) = delete;
		Config(Config&&) = delete;
		Config& operator=(Config&&) = delete;
	public:
		void Init();
		void PrintConfig() const;
	public:
		int GetFPS() const { return this->fps; }
	public:
		std::string configFileDirectory = "assets/config";//配置文件目录
		std::string configFileName = "config.json";//配置文件名字
		std::string configFilePath;//配置文件路径
		//配置项
		//	窗口相关配置
		std::string windowTitle = "Engin Test Application";//窗口标题
		SDL_Rect windowRect = { 100,100,800,600 };//窗口位置和大小
		bool isFullScreen = false;//是否全屏
		//	渲染相关配置
		int fps = 100;//帧率
		bool enableVSync = true;//是否启用垂直同步
		//	按键相关配置
		std::unordered_map<std::string, std::vector<std::string>> keyMap = {
			{"MoveUp", {"W", "UP"}},
			{"MoveDown", {"S", "DOWN"}},
			{"MoveLeft", {"A", "LEFT"}},
			{"MoveRight", {"D", "RIGHT"}},
			{"Jump",{"SPACE"}},
			{"FullScreen",{"F11"}},
			{"Quit", {"ESCAPE"}},
			{"Attack", {"K"}}
		};//按键绑定
	public:
		bool LoadConfigFile(const std::string& filePath);
		[[nodiscard]] bool SaveConfigFile(const std::string& filePath);
	private:
		bool LoadConfigFromJSON(const nlohmann::json& jsonData);
		bool SaveConfigToJSON(nlohmann::json& jsonData) const;
	private:
		const char* GetClassName() const {
			return typeid(*this).name();
		}
	};
}

