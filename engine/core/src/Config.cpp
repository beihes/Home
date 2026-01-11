#include "../inc/Config.h"
#include "utils/file/inc/File.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::core {
	engine::core::Config::Config(const std::string& fileDirectory)
	{
		this->configFileDirectory = fileDirectory;
		this->configFilePath = this->configFileDirectory + '/' + this->configFileName;
		if (!this->LoadConfigFile(this->configFilePath)) {
			log::Debug("[{}]创建配置文件目录", this->GetClassName());
			if (!file::CreateDirectory(fileDirectory))
			{
				log::Error("[{}]创建配置文件目录失败", this->GetClassName());
				log::Warn("[{}]配置文件初始为默认值", this->GetClassName());
				return;
			}
			if (!this->SaveConfigFile(this->configFilePath))
			{
				log::Error("[{}]保存配置文件失败", this->GetClassName());
				log::Warn("[{}]配置文件初始为默认值", this->GetClassName());
			}
		}
	}
	void Config::Init()
	{

	}
	void Config::PrintConfig() const
	{
	}
	bool Config::LoadConfigFile(const std::string& filePath)
	{
		SDL_IOStream* ioStream = SDL_IOFromFile(filePath.c_str(), "r");
		if (!ioStream)
		{
			log::Error("[{}]配置文件打开失败: {}", this->GetClassName(), SDL_GetError());
			return false;
		}
		Sint64 ioSize = SDL_GetIOSize(ioStream);
		if (ioSize <= 0)
		{
			log::Error("[{}]配置文件为空: {}", this->GetClassName(), SDL_GetError());
			SDL_CloseIO(ioStream);
			return false;
		}
		// 读取文件内容到字符串
		std::string fileData;
		fileData.resize(ioSize);
		Sint64 readBytes = SDL_ReadIO(ioStream, fileData.data(), ioSize);
		SDL_CloseIO(ioStream);
		if (readBytes != ioSize) {
			log::Error("[{}]读取配置文件失败: {}", this->GetClassName(), SDL_GetError());
			return false;
		}
		log::Debug("[{}]读取配置文件成功: {}", this->GetClassName(), fileData.size());
		// 解析JSON数据
		nlohmann::json jsonData;
		try {
			jsonData = nlohmann::json::parse(fileData);
			LoadConfigFromJSON(jsonData);
		}
		catch (const nlohmann::json::parse_error& e) {
			log::Error("[{}]JSON parse 失败: {}", this->GetClassName(), e.what());
			return false;
		}
		jsonData.clear();
		log::Debug("[{}]应用配置成功", this->GetClassName());
		return true;
	}
	bool Config::SaveConfigFile(const std::string& filePath)
	{
		SDL_IOStream* file = SDL_IOFromFile(filePath.c_str(), "w"); // 写二进制
		if (!file) {
			log::Error("[{}]打开配置文件失败: {}", this->GetClassName(), SDL_GetError());
			return false;
		}
		nlohmann::json jsonData;
		SaveConfigToJSON(jsonData);
		std::string content = jsonData.dump(4); // 美化输出，缩进4个空格
		Sint64 writeSize = SDL_WriteIO(file, content.data(), content.size());
		SDL_CloseIO(file);

		if (writeSize != (Sint64)content.size()) {
			log::Error("[{}]保存配置文件失败: {}", this->GetClassName(), SDL_GetError());
			return false;
		}
		log::Debug("[{}]保存配置文件成功", this->GetClassName());
		return true;
	}
	
	bool Config::LoadConfigFromJSON(const nlohmann::json& jsonData)
	{
		if (jsonData.contains("window") && jsonData["window"].is_object())
		{
			const auto& windowData = jsonData.at("window");
			this->windowTitle = windowData.value("title", this->windowTitle);
			this->windowRect.x = windowData.value("x", this->windowRect.x);
			this->windowRect.y = windowData.value("y", this->windowRect.y);
			this->windowRect.w = windowData.value("w", this->windowRect.w);
			this->windowRect.h = windowData.value("h", this->windowRect.h);
			this->isFullScreen = windowData.value("isFullScreen", this->isFullScreen);
		}
		if (jsonData.contains("render") && jsonData["render"].is_object())
		{
			const auto& renderData = jsonData.at("render");
			this->fps = renderData.value("fps", fps);
			this->enableVSync = renderData.value("enableVSync", this->enableVSync);
		}
		if (jsonData.contains("keyMap") && jsonData["keyMap"].is_object())
		{
			const auto& keyMapData = jsonData.at("keyMap");
			auto midKeyMap = keyMapData.get <std::unordered_map<std::string, std::vector<std::string>>>();
			for (const auto& [action, keys] : midKeyMap)
			{
				this->keyMap[action] = keys;
			}
		}
		return true;
	}
	bool Config::SaveConfigToJSON(nlohmann::json& jsonData) const
	{
		jsonData["window"] = {
			{"title", this->windowTitle},
			{"x", this->windowRect.x},
			{"y", this->windowRect.y},
			{"w", this->windowRect.w},
			{"h", this->windowRect.h},
			{"isFullScreen", this->isFullScreen}
		};
		jsonData["render"] = {
			{"fps", this->fps},
			{"enableVSync", this->enableVSync}
		};
		nlohmann::json keyMapData;
		for (const auto& [action, keys] : this->keyMap)
		{
			nlohmann::json keyArray = nlohmann::json::array();
			for (const auto& key : keys)
			{
				keyArray.push_back(static_cast<std::string>(key));
			}
			keyMapData[action] = keyArray;
		}
		jsonData["keyMap"] = keyMapData;
		return true;
	}
}