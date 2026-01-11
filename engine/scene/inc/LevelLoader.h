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

#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace engine::component {
    struct TileInfo;
    enum class TileType;
}

namespace engine::scene {
    class Scene;

    /**
     * @brief 负责从 Tiled JSON 文件 (.tmj) 加载关卡数据到 Scene 中。
     */
    class LevelLoader final {
    public:
        LevelLoader() = default;
        ~LevelLoader() = default;
        /**
         * @brief 加载关卡数据到指定的 Scene 对象中。
         * @param levelPath Tiled JSON 地图文件的路径。
         * @param scene 要加载数据的目标 Scene 对象。
         * @return bool 是否加载成功。
         */
        [[nodiscard]] bool LoadLevel(const std::string& levelPath, Scene& scene);

    private:
        void LoadImageLayer(const nlohmann::json& layerJson, Scene& scene);    ///< @brief 加载图片图层
        void LoadTileLayer(const nlohmann::json& layerJson, Scene& scene);     ///< @brief 加载瓦片图层
        void LoadObjectLayer(const nlohmann::json& layerJson, Scene& scene);   ///< @brief 加载对象图层

        /**
        * @brief 获取瓦片属性
        * @tparam T 属性类型
        * @param tileJson 瓦片json数据
        * @param propertyName 属性名称
        * @return 属性值，如果属性不存在则返回 std::nullopt
        */
        template<typename T>
        std::optional<T> GetTileProperty(const nlohmann::json& tileJson, const std::string& propertyName) {
            if (!tileJson.contains("properties")) return std::nullopt;
            const auto& properties = tileJson["properties"];
            for (const auto& property : properties) {
                if (property.contains("name") && property["name"] == propertyName) {
                    if (property.contains("value")) {
                        return property["value"].get<T>();
                    }
                }
            }
            return std::nullopt;
        }

        /**
         * @brief 获取瓦片碰撞器矩形
         * @param tileJson 瓦片json数据
         * @return 碰撞器矩形，如果碰撞器不存在则返回 std::nullopt
         */
        std::optional<SDL_FRect> GetColliderRect(const nlohmann::json& tileJson);

        /**
        * @brief 根据瓦片json对象获取瓦片类型
        * @param tileJson 瓦片json数据
        * @return 瓦片类型
        */
        engine::component::TileType GetTileType(const nlohmann::json& tileJson);
        /**
        * @brief 根据（单一图片）图块集中的id获取瓦片类型
        * @param tileset_json 图块集json数据
        * @param local_id 图块集中的id
        * @return 瓦片类型
        */
        component::TileType GetTileTypeById(const nlohmann::json& tilesetJson, int localId);

        /**
        * @brief 根据全局 ID 获取瓦片信息。
        * @param gid 全局 ID。
        * @return engine::component::TileInfo 瓦片信息。
        */
        component::TileInfo GetTileInfoByGid(int gid);

        /**
         * @brief 根据全局 ID 获取瓦片json对象 (用于对象层获取瓦片信息)
         * @param gid 全局 ID
         * @return 瓦片json对象
         */
        std::optional<nlohmann::json> GetTileJsonByGid(int gid) const;

        /**
        * @brief 加载 Tiled tileset 文件 (.tsj)。
        * @param tileset_path Tileset 文件路径。
        * @param first_gid 此 tileset 的第一个全局 ID。
        */
        void LoadTileset(const std::string& tilesetPath, int firstGid);

        /**
         * @brief 解析图片路径，合并地图路径和相对路径。例如：
         * 1. 文件路径："assets/maps/level1.tmj"
         * 2. 相对路径："../textures/Layers/back.png"
         * 3. 最终路径："assets/textures/Layers/back.png"
         * @param relative_path 相对路径（相对于文件）
         * @param file_path 文件路径
         * @return std::string 解析后的完整路径。
         */
        std::string ResolvePath(const std::string& imagePath);
    public:
        std::string mapPath_;      ///< @brief 地图路径（拼接路径时需要）
        SDL_Point mapSize_;       ///< @brief 地图尺寸(瓦片数量)
        SDL_Point tileSize_;      ///< @brief 瓦片尺寸(像素)
        std::map<int, nlohmann::json> tilesetData_;    ///< @brief firstgid -> 瓦片集数据
    private:
        const char* GetClassName() const {
            return typeid(*this).name();
        }
    };

} // namespace engine::scene