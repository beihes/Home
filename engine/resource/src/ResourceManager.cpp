#include "../inc/ResourceManager.h"
#include "../inc/AudioManager.h"
#include "../inc/FontManager.h"
#include "../inc/TextureManager.h"

namespace engine::resource
{
	ResourceManager::~ResourceManager() = default;
	ResourceManager::ResourceManager(SDL_Renderer *renderer)
	{
		this->audioManager_ = std::make_unique<AudioManager>();
		this->fontManager_ = std::make_unique<FontManager>();
		this->textureManager_ = std::make_unique<TextureManager>(renderer);
	}
	void ResourceManager::Clear()
	{
		this->audioManager_->Clear();
		this->fontManager_->Clear();
		this->textureManager_->Clear();
	}
	void *ResourceManager::GetAudio(const std::string &filePath)
	{
		return nullptr;
	}
	TTF_Font *ResourceManager::GetFont(const std::string &filePath, float fontSize)
	{
		return fontManager_->GetFont(filePath, fontSize);
	}
	SDL_Texture *ResourceManager::GetTexture(const std::string &filePath)
	{
		return textureManager_->GetTexture(filePath);
	}

	SDL_FPoint ResourceManager::GetTextureSize(const std::string& textureId)
	{
		SDL_FPoint midSize;
		SDL_GetTextureSize(this->textureManager_->GetTexture(textureId), &midSize.x, &midSize.y);
		return midSize;
	}

	bool ResourceManager::Unload_Font(const std::string &filePath, float fontSize)
	{
		return this->fontManager_->UnloadFont(filePath, fontSize);
	}
	bool ResourceManager::Unload_Texture(const std::string &filePath)
	{
		return this->textureManager_->Unload_Texture(filePath);
	}
}
