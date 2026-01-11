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

#include "component.h"
#include "engine/render/inc/Sprite.h"
#include "utils/rect/inc/Rect.h"
#include <vector>

using namespace utils::rect;

namespace engine::render {
    class Sprite;
}

namespace engine::core {
    class Context;
}

namespace engine::physics {
    class PhysicsEngine;
}

namespace engine::component {
    /**
     * @brief 定义瓦片的类型，用于游戏逻辑（例如碰撞）。
     */
    enum class TileType {
        EMPTY,      ///< @brief 空白瓦片
        NORMAL,     ///< @brief 普通瓦片
        SOLID,      ///< @brief 静止可碰撞瓦片
        UNISOLID,   ///< @brief 单向静止可碰撞瓦片
        SLOPE_0_1,  ///< @brief 斜坡瓦片，高度:左0  右1
        SLOPE_1_0,  ///< @brief 斜坡瓦片，高度:左1  右0
        SLOPE_0_2,  ///< @brief 斜坡瓦片，高度:左0  右1/2
        SLOPE_2_1,  ///< @brief 斜坡瓦片，高度:左1/2右1
        SLOPE_1_2,  ///< @brief 斜坡瓦片，高度:左1  右1/2
        SLOPE_2_0,  ///< @brief 斜坡瓦片，高度:左1/2右0
        HAZARD,     ///< @brief 危险瓦片（例如火焰、尖刺等）
        LADDER,     ///< @brief 梯子瓦片
        // 未来补充其它类型
    };

    /**
    * @brief 包含单个瓦片的渲染和逻辑信息。
    */
    struct TileInfo {
        render::Sprite sprite;      ///< @brief 瓦片的视觉表示
        TileType type;              ///< @brief 瓦片的逻辑类型
        TileInfo(render::Sprite s = render::Sprite(), TileType t = TileType::EMPTY) : sprite(std::move(s)), type(t) {}
    };
    class TileLayerComponent final : public Component {
        friend class object::Object;
    public:
        TileLayerComponent() = default;
        ~TileLayerComponent() = default;
    public:
        /**
         * @brief 构造函数
         * @param tileSize 单个瓦片尺寸（像素）
         * @param mapSize 地图尺寸（瓦片数）
         * @param tiles 初始化瓦片数据的容器 (会被移动)
         */
        TileLayerComponent(const SDL_Point& tileSize, const SDL_Point& mapSize, std::vector<TileInfo>&& tiles);

        /**
         * @brief 根据瓦片坐标获取瓦片信息
         * @param pos 瓦片坐标 (0 <= x < mapSize_.x, 0 <= y < mapSize_.y)
         * @return const TileInfo* 指向瓦片信息的指针，如果坐标无效则返回 nullptr
         */
        const TileInfo* GetTileInfoAt(const SDL_Point& pos) const;

        /**
         * @brief 根据瓦片坐标获取瓦片类型
         * @param pos 瓦片坐标 (0 <= x < mapSize_.x, 0 <= y < mapSize_.y)
         * @return TileType 瓦片类型，如果坐标无效则返回 TileType::EMPTY
         */
        TileType GetTileTypeAt(const SDL_Point& pos) const;

        /**
         * @brief 根据世界坐标获取瓦片类型
         * @param world_pos 世界坐标
         * @return TileType 瓦片类型，如果坐标无效或对应空瓦片则返回 TileType::EMPTY
         */
        TileType GetTileTypeAtWorldPos(const SDL_Point& world_pos) const;

        // getters and setters
        SDL_Point GetTileSize() const { return this->tileSize_; }               ///< @brief 获取单个瓦片尺寸
        SDL_Point GetMapSize() const { return this->mapSize_; }                 ///< @brief 获取地图尺寸
        SDL_Point GetWorldSize() const { return this->mapSize_ * this->tileSize_; }   ///< @brief 获取地图世界尺寸
        const std::vector<TileInfo>& GetTiles() const { return this->tiles_; }    ///< @brief 获取瓦片容器
        const SDL_FPoint& GetOffset() const { return this->offset_; }              ///< @brief 获取瓦片层的偏移量
        bool IsHidden() const { return this->isHidden_; }                        ///< @brief 获取是否隐藏（不渲染）
    public:
        void SetOffset(SDL_FPoint offset) { this->offset_ = std::move(offset); }       ///< @brief 设置瓦片层的偏移量
        void SetHidden(bool hidden) { this->isHidden_ = hidden; }                ///< @brief 设置是否隐藏（不渲染）
        void SetPhysicsEngine(physics::PhysicsEngine* physicsEngine) { this->physicsEngine_ = physicsEngine; }
    protected:
        // 核心循环方法
        void Init() override;
        void Update(float, core::Context&) override {}
        void Render(core::Context& context) override;
        void Clean() override;

    private:
        SDL_Point tileSize_;                    ///< @brief 单个瓦片尺寸（像素）
        SDL_Point mapSize_;                     ///< @brief 地图尺寸（瓦片数）
        std::vector<TileInfo> tiles_;           ///< @brief 存储所有瓦片信息 (按"行主序"存储, index = y * map_width_ + x)
        SDL_FPoint offset_ = { 0.0f, 0.0f };    ///< @brief 瓦片层在世界中的偏移量 (瓦片层通常不需要缩放及旋转，因此不引入Transform组件)
                                                    // offset_ 最好也保持默认的0，以免增加不必要的复杂性
        bool isHidden_ = false;                 ///< @brief 是否隐藏（不渲染）
        physics::PhysicsEngine* physicsEngine_ = nullptr;   ///< @brief 物理引擎的指针， clean()函数中可能需要反注册
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };
}