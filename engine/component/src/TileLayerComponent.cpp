#include "../inc/TileLayerComponent.h"
#include "object/inc/Object.h"
#include "core/inc/Context.h"
#include "engine/physics/inc/PhysicsEngine.h"
#include "render/inc/Camera.h"
#include "render/inc/Renderer.h"
#include "utils/log/inc/Log.h"

using namespace utils;

namespace engine::component {
	TileLayerComponent::TileLayerComponent(const SDL_Point& tileSize, const SDL_Point& mapSize, std::vector<TileInfo>&& tiles)
		: tileSize_(tileSize), mapSize_(mapSize), tiles_(std::move(tiles))
	{
		if (this->tiles_.size() != static_cast<size_t>(this->mapSize_.x * this->mapSize_.y)) {
			log::Error("[{}]TileLayerComponent: 地图尺寸与提供的瓦片向量大小不匹配。瓦片数据将被清除。", this->GetClassName());
			tiles_.clear();
			this->mapSize_ = { 0, 0 };
		}
		log::Trace("[{}]TileLayerComponent 构造完成", this->GetClassName());
	}

	void TileLayerComponent::Init() {
		if (!this->owner_) { log::Warn("[{}]TileLayerComponent 的 owner_ 未设置。", this->GetClassName()); }
		log::Trace("[{}]TileLayerComponent 初始化完成", this->GetClassName());
	}

	void TileLayerComponent::Render(core::Context& context) {
		if (this->tileSize_.x <= 0 || this->tileSize_.y <= 0) {
			return; // 防止除以零或无效尺寸
		}
		// 遍历所有瓦片
		for (int y = 0; y < this->mapSize_.y; ++y) {
			for (int x = 0; x < this->mapSize_.x; ++x) {
				size_t index = static_cast<size_t>(y) * this->mapSize_.x + x;
				// 检查索引有效性以及瓦片是否需要渲染
				if (index < this->tiles_.size() && this->tiles_[index].type != TileType::EMPTY) {
					const auto& tileInfo = this->tiles_[index];
					// 计算该瓦片在世界中的左上角位置 (drawSprite 预期接收左上角坐标)
					SDL_FPoint tileLeftTopPos = {
						this->offset_.x + static_cast<float>(x) * this->tileSize_.x,
						this->offset_.y + static_cast<float>(y) * this->tileSize_.y
					};
					// 但如果图片的大小与瓦片的大小不一致，需要调整 y 坐标 (瓦片层的对齐点是左下角)
					if (static_cast<int>(tileInfo.sprite.GetSrcRect()->h) != tileSize_.y) {
						tileLeftTopPos.y -= (tileInfo.sprite.GetSrcRect()->h - static_cast<float>(tileSize_.y));
					}
					// 执行绘制
					context.GetRenderer().DrawSprite(context.GetCamera(), tileInfo.sprite, tileLeftTopPos);
				}
			}
		}
	}

	void TileLayerComponent::Clean()
	{
		if (this->physicsEngine_) {
			this->physicsEngine_->UnregisterCollisionLayer(this);
		}
	}

	const TileInfo* TileLayerComponent::GetTileInfoAt(const SDL_Point& pos) const {
		if (pos.x < 0 || pos.x >= this->mapSize_.x || pos.y < 0 || pos.y >= this->mapSize_.y) {
			log::Warn("[{}]TileLayerComponent: 瓦片坐标越界: ({}, {})", this->GetClassName(), pos.x, pos.y);
			return nullptr;
		}
		size_t index = static_cast<size_t>(pos.y * this->mapSize_.x + pos.x);
		// 瓦片索引不能越界
		if (index < this->tiles_.size()) { return &this->tiles_[index]; }
		log::Warn("[{}]TileLayerComponent: 瓦片索引越界: {}", this->GetClassName(), index);
		return nullptr;
	}

	TileType TileLayerComponent::GetTileTypeAt(const SDL_Point& pos) const {
		const TileInfo* info = this->GetTileInfoAt(pos);
		return info ? info->type : TileType::EMPTY;
	}

	TileType TileLayerComponent::GetTileTypeAtWorldPos(const SDL_Point& worldPos) const {
		SDL_FPoint relativePos = worldPos - this->offset_;
		int tileX = static_cast<int>(std::floor(relativePos.x / this->tileSize_.x));
		int tileY = static_cast<int>(std::floor(relativePos.y / this->tileSize_.y));
		return this->GetTileTypeAt(SDL_Point{ tileX, tileY });
	}
}