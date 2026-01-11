#include "../inc/FontManager.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::resource{
	FontManager::FontManager()
	{
		this->Init();
	}
	FontManager::~FontManager() {
		this->Clear();
		TTF_Quit();
	}
	void FontManager::Init() {
		if (!TTF_WasInit() && !TTF_Init())
		{
			log::Error("[{}]SDL_ttf init failed",this->GetClassName());
		}
	}
	void FontManager::Clear()
	{
		if (this->fonts_.empty())
			return;
		log::Debug("[{}]删除字体：{}", this->GetClassName(), this->fonts_.size());
		this->fonts_.clear();
	}
	TTF_Font* FontManager::Load_Font_From_File(const std::string& filePath, float fontSize)
	{
		if (fontSize <= 0)
		{
			log::Error("[{}]fontSize 无效 : {}", this->GetClassName(), filePath);
			return nullptr;
		}
		FontKey fontKey = FontKey(filePath, fontSize);
		auto midData = this->fonts_.find(fontKey);
		if (midData != this->fonts_.end())
		{
			return midData->second.get();
		}
		TTF_Font* midFont = TTF_OpenFont(filePath.c_str(), fontSize);
		if (!midFont)
		{
			log::Error("[{}]can not load font : {}", this->GetClassName(), filePath);
			return nullptr;
		}
		this->fonts_.emplace(fontKey, std::unique_ptr<TTF_Font, TTF_FontDelete>(midFont));
		log::Debug("[{}]读取字体成功：[ {}, {} ]", this->GetClassName(), filePath, fontSize);
		return midFont;
	}
	TTF_Font* FontManager::GetFont(const std::string& filePath, float fontSize)
	{
		if (fontSize <= 0)
		{
			log::Error("[{}]fontSize 无效 : {}", this->GetClassName(), filePath);
			return nullptr;
		}
		auto midData = this->fonts_.find(std::pair<std::string, float>(filePath, fontSize));
		if (midData == this->fonts_.end())
		{
			log::Warn("[{}]can not find font : {}",this->GetClassName(), filePath);
			return this->Load_Font_From_File(filePath, fontSize);
		}
		return midData->second.get();
	}
	bool FontManager::UnloadFont(const std::string& filePath, float fontSize)
	{
		if (fontSize <= 0)
		{
			log::Error("[{}]fontSize 无效 : {}", this->GetClassName(), filePath);
			return false;
		}
		auto midData = this->fonts_.find(std::pair<std::string, float>(filePath, fontSize));
		if (midData == this->fonts_.end())
		{
			log::Error("[{}]can not find font : {}", this->GetClassName(), filePath);
			return false;
		}
		log::Debug("[{}]删除字体成功：[ {}, {} ]", this->GetClassName(), filePath, fontSize);
		this->fonts_.erase(midData);
		return true;
	}
}
