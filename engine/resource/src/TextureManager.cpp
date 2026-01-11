#include "../inc/TextureManager.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::resource {
	TextureManager::TextureManager(SDL_Renderer* renderer) :renderer_(renderer)
	{
		if (!renderer)
		{
			log::Error("[{}]renderer is null", this->GetClassName());
			return;
		}
	}
	TextureManager::~TextureManager() = default;
	void TextureManager::Init() {

	}
	void TextureManager::Clear()
	{
		if (textures_.empty())
			return;
		log::Debug("[{}]删除纹理：{}\r\n", this->GetClassName(), textures_.size());
		textures_.clear();
	}
	SDL_Texture* TextureManager::Load_Texture_From_File(const std::string& filePath)
	{
		auto midData = textures_.find(filePath);
		if (midData != textures_.end())
		{
			return midData->second.get();
		}
		SDL_Texture* midTexture = IMG_LoadTexture(renderer_, filePath.c_str());
		if (!SDL_SetTextureScaleMode(midTexture, SDL_SCALEMODE_NEAREST)) { log::Warn("[{}]无法设置纹理缩放模式为最邻近插值", this->GetClassName()); }
		if (!midTexture)
		{
			log::Error("[{}]can not load texture : {}", this->GetClassName(), filePath);
			return nullptr;
		}
		this->textures_.emplace(filePath, std::unique_ptr<SDL_Texture, SDL_TextureDelete>(midTexture));
		log::Debug("[{}]从文件读取texture成功：{}", this->GetClassName(), filePath);
		return midTexture;
	}
	SDL_Texture* TextureManager::GetTexture(const std::string& filePath)
	{
		auto midData = textures_.find(filePath);
		if (midData == textures_.end())
		{
			log::Warn("[{}]can not find texture", this->GetClassName());
			return Load_Texture_From_File(filePath);	
		}
		return midData->second.get();
	}
	bool TextureManager::Unload_Texture(const std::string& filePath)
	{
		auto midData = textures_.find(filePath);
		if (midData == textures_.end())
		{
			log::Error("[{}]can not find texture", this->GetClassName());
			return false;
		}
		log::Debug("[{}]删除纹理：{}", this->GetClassName(), filePath);
		textures_.erase(midData);
		return false;
	}
}
