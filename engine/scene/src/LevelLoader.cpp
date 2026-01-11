#include "../inc/LevelLoader.h"
#include "../inc/Scene.h"
#include "engine/component/inc/ParallaxComponent.h"
#include "engine/component/inc/PhysicsComponent.h"
#include "engine/component/inc/SpriteComponent.h"
#include "engine/component/inc/TileLayerComponent.h"
#include "engine/component/inc/TransformComponent.h"
#include "engine/core/inc/Context.h"
#include "engine/object/inc/Object.h"
#include "engine/resource/inc/ResourceManager.h"
#include "engine/render/inc/Sprite.h"
#include "utils/file/inc/File.h"
#include "utils/log/inc/Log.h"
#include "utils/rect/inc/Rect.h"
#include <filesystem>
#include <fstream>

using namespace utils;

namespace engine::scene
{
    bool LevelLoader::LoadLevel(const std::string& levelPath, Scene& scene) {
        // 1. 加载 JSON 文件
        std::string fileData;
        this->mapPath_ = levelPath;
        if (!file::ReadFile(levelPath, fileData)) { return false; }

        // 2. 解析 JSON 数据
        nlohmann::json jsonData;
        try {
            jsonData = nlohmann::json::parse(fileData);
        }
        catch (const nlohmann::json::parse_error& e) {
            log::Error("[{}]JSON parse 失败: {}", this->GetClassName(), e.what());
            return false;
        }
        // // 3. 获取基本地图信息 (名称、地图尺寸、瓦片尺寸)
        this->mapSize_ = SDL_Point{ jsonData.value("width", 0), jsonData.value("height", 0) };
        this->tileSize_ = SDL_Point{ jsonData.value("tilewidth", 0), jsonData.value("tileheight", 0) };

        // 4. 加载 tileset 数据
        if (jsonData.contains("tilesets") && jsonData["tilesets"].is_array()) {
            for (const auto& tilesetJson : jsonData["tilesets"]) {
                if (!tilesetJson.contains("source") || !tilesetJson["source"].is_string() ||
                    !tilesetJson.contains("firstgid") || !tilesetJson["firstgid"].is_number_integer()) {
                    log::Error("[{}]tilesets 对象中缺少有效 'source' 或 'firstgid' 字段。", this->GetClassName());
                    continue;
                }
                auto tilesetPath = this->ResolvePath(tilesetJson["source"].get<std::string>());  // 支持隐式转换，可以省略.get<T>()方法，
                auto first_gid = tilesetJson["firstgid"];
                this->LoadTileset(tilesetPath, first_gid);
            }
        }

        // 5. 加载图层数据
        if (!jsonData.contains("layers") || !jsonData["layers"].is_array()) {       // 地图文件中必须有 layers 数组
            log::Error("[{}]地图文件 '{}' 中缺少或无效的 'layers' 数组。", this->GetClassName(), levelPath);
            return false;
        }
        for (const auto& layerJson : jsonData["layers"]) {
            // 获取各图层对象中的类型（type）字段
            std::string layerType = layerJson.value("type", "none");
            if (!layerJson.value("visible", true)) {
                log::Info("[{}]图层 '{}' 不可见，跳过加载。", this->GetClassName(), layerJson.value("name", "Unnamed"));
                continue;
            }
            // 根据图层类型决定加载方法
            if (layerType == "imagelayer") {
                this->LoadImageLayer(layerJson, scene);
            } else if (layerType == "tilelayer") {
                this->LoadTileLayer(layerJson, scene);
            } else if (layerType == "objectgroup") {
                this->LoadObjectLayer(layerJson, scene);
            } else { log::Warn("[{}]不支持的图层类型: {}", this->GetClassName(), layerType); }
        }
        log::Info("[{}]关卡加载完成: {}", this->GetClassName(), levelPath);
        return true;
    }

    void LevelLoader::LoadImageLayer(const nlohmann::json& layerJson, Scene& scene) {
        // 获取纹理相对路径 （会自动处理'\/'符号）
        std::string imagePath = layerJson.value("image", "");     // json.value()返回的是一个临时对象，需要赋值才能保存，
        // 不能用std::string_view
        if (imagePath.empty()) {
            log::Error("[{}]图层 '{}' 缺少 'image' 属性。", this->GetClassName(), layerJson.value("name", "Unnamed"));
            return;
        }
        auto textureId = this->ResolvePath(imagePath);

        // 获取图层偏移量（json中没有则代表未设置，给默认值即可）
        const SDL_FPoint offset = SDL_FPoint{ layerJson.value("offsetx", 0.0f), layerJson.value("offsety", 0.0f) };

        // 获取视差因子及重复标志
        const SDL_FPoint scrollFactor = SDL_FPoint{ layerJson.value("parallaxx", 1.0f), layerJson.value("parallaxy", 1.0f) };
        const SDL_BPoint repeat = SDL_BPoint{ layerJson.value("repeatx", false), layerJson.value("repeaty", false) };

        // 获取图层名称
        std::string layerName = layerJson.value("name", "Unnamed");

        /*  可用类似方法获取其它各种属性，这里我们暂时用不上 */

        // 创建游戏对象
        auto object = std::make_unique<object::Object>(layerName);
        // 依次添加Transform，Parallax组件
        object->AddComponent<component::TransformComponent>(offset);
        object->AddComponent<component::ParallaxComponent>(textureId, scrollFactor, repeat);
        // 添加到场景中
        scene.AddObject(std::move(object));
        log::Info("[{}]加载图层: '{}' 完成", this->GetClassName(), layerName);
    }

    void LevelLoader::LoadTileLayer(const nlohmann::json& layerJson, Scene& scene)
    {
        if (!layerJson.contains("data") || !layerJson["data"].is_array()) {
            log::Error("[{}]图层 '{}' 缺少 'data' 属性。", this->GetClassName(), layerJson.value("name", "Unnamed"));
            return;
        }
        // 准备 TileInfo Vector (瓦片数量 = 地图宽度 * 地图高度)
        std::vector<component::TileInfo> tiles;
        tiles.reserve(this->mapSize_.x * this->mapSize_.y);

        // 获取图层数据 (瓦片 ID 列表)
        const auto& data = layerJson["data"];

        // 根据gid获取必要信息，并依次填充 TileInfo Vector
        for (const auto& gid : data) {
            tiles.push_back(this->GetTileInfoByGid(gid));
        }

        // 获取图层名称
        std::string layerName = layerJson.value("name", "Unnamed");
        // 创建游戏对象
        auto object = std::make_unique<object::Object>(layerName);
        // 添加Tilelayer组件
        object->AddComponent<component::TileLayerComponent>(this->tileSize_, this->mapSize_, std::move(tiles));
        // 添加到场景中
        scene.AddObject(std::move(object));
        log::Info("[{}]加载瓦片图层: '{}' 完成", this->GetClassName(), layerName);
    }

    void LevelLoader::LoadObjectLayer(const nlohmann::json& layerJson, Scene& scene)
    {
        if (!layerJson.contains("objects") || !layerJson["objects"].is_array()) {
            log::Error("[{}]对象图层 '{}' 缺少 'objects' 属性。", this->GetClassName(), layerJson.value("name", "Unnamed"));
            return;
        }
        // 获取对象数据
        const auto& objects = layerJson["objects"];
        // 遍历对象数据
        for (const auto& object : objects) {
            // 获取对象gid
            auto gid = object.value("gid", 0);
            if (gid == 0) {     // 如果gid为0 (即不存在)，则代表自己绘制的形状
                //TODO
            } else {// 如果gid存在，则按照图片解析流程
                // --- 根据gid获取必要信息，每个gid对应一个游戏对象 ---
                auto tileInfo = this->GetTileInfoByGid(gid);
                if (tileInfo.sprite.GetTextureId().empty()) {
                    log::Error("[{}]gid为 {} 的瓦片没有图像纹理。", this->GetClassName(), gid);
                    continue;
                }
                // 获取Transform相关信息
                auto position = SDL_FPoint{ object.value("x", 0.0f), object.value("y", 0.0f) };
                auto dst_size = SDL_FPoint{ object.value("width", 0.0f), object.value("height", 0.0f) };
                position = SDL_FPoint{ position.x, position.y - dst_size.y };  // 实际position需要进行调整(左下角到左上角)

                auto rotation = object.value("rotation", 0.0f);
                auto src_size_opt = tileInfo.sprite.GetSrcRect();
                if (!src_size_opt) {        // 正常情况下，所有瓦片的Sprite都设置了源矩形，没有代表某处出错
                    log::Error("[{}]gid为 {} 的瓦片没有源矩形。", this->GetClassName(), gid);
                    continue;
                }
                auto src_size = SDL_FPoint{ src_size_opt->w, src_size_opt->h };    // 成员变量除了 value().w 外，也可以这样获取
                auto scale = dst_size / src_size;

                // 获取对象名称
                std::string objectName = object.value("name", "Unnamed");

                // 创建游戏对象并添加组件
                auto object = std::make_unique<object::Object>(objectName);
                object->AddComponent<component::TransformComponent>(position, scale, rotation);
                object->AddComponent<component::SpriteComponent>(std::move(tileInfo.sprite), scene.GetContext().GetResourceManager());

                // 获取瓦片json信息      1. 必然存在，因为getTileInfoByGid(gid)函数已经顺利执行
                                // 2. 这里再获取json，实际上检索了两次，未来可以优化
                auto tileJson = this->GetTileJsonByGid(gid);
                //TODO: box2d的初始化
                // 获取碰信息：如果是SOLID类型，则添加物理组件，且图片源矩形区域就是碰撞盒大小
                if (tileInfo.type == component::TileType::SOLID) {
                    // 物理组件不受重力影响
                    object->AddComponent<component::PhysicsComponent>(&scene.GetContext().GetPhysicsEngine());

                    // 设置标签方便物理引擎检索
                    object->SetTag("solid");
                }
                // 如果非SOLID类型，检查自定义碰撞盒是否存在
                else if (auto rect = this->GetColliderRect(tileJson); rect) {
                    // 如果有，添加碰撞组件
                    // cc->SetOffset(SDL_FPoint(rect->x, rect->y));  // 自定义碰撞盒的坐标是相对于图片坐标，也就是针对Transform的偏移量
                    // 和物理组件（默认不受重力影响）
                    object->AddComponent<component::PhysicsComponent>(&scene.GetContext().GetPhysicsEngine());
                }

                // 获取标签信息并设置
                auto tag = this->GetTileProperty<std::string>(tileJson, "tag");
                if (tag) {
                    object->SetTag(tag.value());
                }

                // 获取重力信息并设置
                auto gravity = this->GetTileProperty<bool>(tileJson, "gravity");
                if (gravity) {
                    auto pc = object->GetComponent<component::PhysicsComponent>();
                    if (pc) {
                        pc->SetUseGravity(gravity.value());
                    } else {
                        log::Warn("[{}]对象 '{}' 在设置重力信息时没有物理组件，请检查地图设置。", this->GetClassName(), objectName.c_str());
                        object->AddComponent<component::PhysicsComponent>(&scene.GetContext().GetPhysicsEngine());
                    }
                }
                // 添加到场景中
                scene.AddObject(std::move(object));
                log::Info("[{}]加载对象: '{}' 完成", this->GetClassName(), objectName);
            }
        }
    }

    std::optional<SDL_FRect> LevelLoader::GetColliderRect(const nlohmann::json& tileJson)
    {
        if (!tileJson.contains("objectgroup")) return std::nullopt;
        auto& objectgroup = tileJson["objectgroup"];
        if (!objectgroup.contains("objects")) return std::nullopt;
        auto& objects = objectgroup["objects"];
        for (const auto& object : objects) {    // 一个图片只支持一个碰撞器。如果有多个，则返回第一个不为空的
            auto rect = SDL_FRect(object.value("x", 0.0f), object.value("y", 0.0f), object.value("width", 0.0f), object.value("height", 0.0f));
            if (rect.x > 0 && rect.y > 0) { return rect; }
        }
        return std::nullopt;    // 如果没找到碰撞器，则返回空
    }


    component::TileType LevelLoader::GetTileType(const nlohmann::json& tileJson)
    {
        if (tileJson.contains("properties")) {
            auto& properties = tileJson["properties"];
            for (auto& property : properties) {
                if (property.contains("name") && property["name"] == "solid") {
                    auto isSolid = property.value("value", false);
                    return isSolid ? component::TileType::SOLID : component::TileType::NORMAL;
                } else if (property.contains("name") && property["name"] == "slope") {
                    auto slopeType = property.value("value", "");
                    if (slopeType == "0_1") {
                        return engine::component::TileType::SLOPE_0_1;
                    } else if (slopeType == "1_0") {
                        return engine::component::TileType::SLOPE_1_0;
                    } else if (slopeType == "0_2") {
                        return engine::component::TileType::SLOPE_0_2;
                    } else if (slopeType == "2_0") {
                        return engine::component::TileType::SLOPE_2_0;
                    } else if (slopeType == "2_1") {
                        return engine::component::TileType::SLOPE_2_1;
                    } else if (slopeType == "1_2") {
                        return engine::component::TileType::SLOPE_1_2;
                    } else {
                        log::Error("[{}]未知的斜坡类型: {}", this->GetClassName(), slopeType);
                        return engine::component::TileType::NORMAL;
                    }
                } else if (property.contains("name") && property["name"] == "unisolid") {
                    auto isUnisolid = property.value("value", false);
                    return isUnisolid ? engine::component::TileType::UNISOLID : engine::component::TileType::NORMAL;
                } else if (property.contains("name") && property["name"] == "hazard") {
                    auto is_hazard = property.value("value", false);
                    return is_hazard ? engine::component::TileType::HAZARD : engine::component::TileType::NORMAL;
                } else if (property.contains("name") && property["name"] == "ladder") {
                    auto is_ladder = property.value("value", false);
                    return is_ladder ? engine::component::TileType::LADDER : engine::component::TileType::NORMAL;
                }
                // TODO: 可以在这里添加更多的自定义属性处理逻辑
            }
        }
        return component::TileType::NORMAL;
    }

    component::TileType LevelLoader::GetTileTypeById(const nlohmann::json& tilesetJson, int localId)
    {
        if (tilesetJson.contains("tiles")) {
            auto& tiles = tilesetJson["tiles"];
            for (auto& tile : tiles) {
                if (tile.contains("id") && tile["id"] == localId) {
                    return this->GetTileType(tile);
                }
            }
        }
        return component::TileType::NORMAL;
    }

    component::TileInfo LevelLoader::GetTileInfoByGid(int gid)
    {
        if (gid == 0) { return component::TileInfo(); }
        // upper_bound：查找tileset_data_中键大于 gid 的第一个元素，返回迭代器
        auto tilesetIt = this->tilesetData_.upper_bound(gid);
        if (tilesetIt == this->tilesetData_.begin()) {
            log::Error("[{}]gid为 {} 的瓦片未找到图块集。", this->GetClassName(), gid);
            return component::TileInfo();
        }
        --tilesetIt;  // 前移一个位置，这样就得到不大于gid的最近一个元素（我们需要的）
        const auto& tileset = tilesetIt->second;
        auto localId = gid - tilesetIt->first;        // 计算瓦片在图块集中的局部ID
        std::string file_path = tileset.value("file_path", "");       // 获取图块集文件路径
        if (file_path.empty()) {
            log::Error("[{}]Tileset 文件 '{}' 缺少 'file_path' 属性。", this->GetClassName(), tilesetIt->first);
            return component::TileInfo();
        }
        // 图块集分为两种情况，需要分别考虑
        if (tileset.contains("image")) {    // 这是单一图片的情况
            // 获取图片路径
            auto textureId = this->ResolvePath(tileset["image"].get<std::string>());
            // 计算瓦片在图片网格中的坐标
            auto coordinate_x = localId % tileset["columns"].get<int>();
            auto coordinate_y = localId / tileset["columns"].get<int>();
            // 根据坐标确定源矩形
            SDL_FRect textureRect = {
                static_cast<float>(coordinate_x * this->tileSize_.x),
                static_cast<float>(coordinate_y * this->tileSize_.y),
                static_cast<float>(this->tileSize_.x),
                static_cast<float>(this->tileSize_.y)
            };
            engine::render::Sprite sprite{ textureId, textureRect };
            auto tileType = this->GetTileTypeById(tileset, localId);   // 获取瓦片类型（只有瓦片id，还没找具体瓦片json）
            return component::TileInfo(sprite, tileType);
        } else {   // 这是多图片的情况
            if (!tileset.contains("tiles")) {   // 没有tiles字段的话不符合数据格式要求，直接返回空的瓦片信息
                log::Error("[{}]Tileset 文件 '{}' 缺少 'tiles' 属性。", this->GetClassName(), tilesetIt->first);
                return engine::component::TileInfo();
            }
            // 遍历tiles数组，根据id查找对应的瓦片
            const auto& tilesJson = tileset["tiles"];
            for (const auto& tileJson : tilesJson) {
                auto tileId = tileJson.value("id", 0);
                if (tileId == localId) {   // 找到对应的瓦片，进行后续操作
                    if (!tileJson.contains("image")) {   // 没有image字段的话不符合数据格式要求，直接返回空的瓦片信息
                        log::Error("[{}]Tileset 文件 '{}' 中瓦片 {} 缺少 'image' 属性。", this->GetClassName(), tilesetIt->first, tileId);
                        return component::TileInfo();
                    }
                    // --- 接下来根据必要信息创建并返回 TileInfo ---
                    // 获取图片路径
                    auto textureId = this->ResolvePath(tileJson["image"].get<std::string>());
                    // 先确认图片尺寸
                    auto imageWidth = tileJson.value("imagewidth", 0);
                    auto imageHeight = tileJson.value("imageheight", 0);
                    // 从json中获取源矩形信息
                    SDL_FRect textureRect = {      // tiled中源矩形信息只有设置了才会有值，没有就是默认值
                        static_cast<float>(tileJson.value("x", 0)),
                        static_cast<float>(tileJson.value("y", 0)),
                        static_cast<float>(tileJson.value("width", imageWidth)),    // 如果未设置，则使用图片尺寸
                        static_cast<float>(tileJson.value("height", imageHeight))
                    };
                    render::Sprite sprite{ textureId, textureRect };
                    auto tileType = this->GetTileType(tileJson);    // 获取瓦片类型（已经有具体瓦片json了）
                    return component::TileInfo(sprite, tileType);
                }
            }
        }
        // 如果能走到这里，说明查找失败，返回空的瓦片信息
        log::Error("[{}]图块集 '{}' 中未找到gid为 {} 的瓦片。", this->GetClassName(), tilesetIt->first, gid);
        return component::TileInfo();
    }

    std::optional<nlohmann::json> LevelLoader::GetTileJsonByGid(int gid) const
    {
        // 1. 查找tileset_data_中键小于等于gid的最近元素
        auto tilesetIt = this->tilesetData_.upper_bound(gid);
        if (tilesetIt == this->tilesetData_.begin()) {
            log::Error("[{}]gid为 {} 的瓦片未找到图块集。", this->GetClassName(), gid);
            return std::nullopt;
        }
        --tilesetIt;
        // 2. 获取图块集json对象
        const auto& tileset = tilesetIt->second;
        auto localId = gid - tilesetIt->first;        // 计算瓦片在图块集中的局部ID
        if (!tileset.contains("tiles")) {   // 没有tiles字段的话不符合数据格式要求，直接返回空
            log::Error("[{}]Tileset 文件 '{}' 缺少 'tiles' 属性。", this->GetClassName(), tilesetIt->first);
            return std::nullopt;
        }
        // 3. 遍历tiles数组，根据id查找对应的瓦片并返回瓦片json
        const auto& tilesJson = tileset["tiles"];
        for (const auto& tileJson : tilesJson) {
            auto tileId = tileJson.value("id", 0);
            if (tileId == localId) {   // 找到对应的瓦片，返回瓦片json
                return { tileJson };
            }
        }
        return std::nullopt;
    }

    void LevelLoader::LoadTileset(const std::string& tilesetPath, int firstGid)
    {
        // 1. 加载 JSON 文件
        std::string fileData;
        if (!file::ReadFile(tilesetPath, fileData)) {
            log::Error("[{}]无法打开 Tileset 文件: {}", this->GetClassName(), tilesetPath);
            return;
        }
        nlohmann::json tsJson;
        try {
            tsJson = nlohmann::json::parse(fileData);
        }
        catch (const nlohmann::json::parse_error& e) {
            log::Error("[{}]解析 Tileset JSON 文件 '{}' 失败: {} (at byte {})", this->GetClassName(), tilesetPath, e.what(), e.byte);
            return;
        }
        tsJson["file_path"] = tilesetPath;    // 将文件路径存储到json中，后续解析图片路径时需要
        this->tilesetData_[firstGid] = std::move(tsJson);
        log::Info("[{}]Tileset 文件 '{}' 加载完成，firstgid: {}", this->GetClassName(), tilesetPath, firstGid);
    }

    std::string LevelLoader::ResolvePath(const std::string& imagePath)
    {
        // try {
        //     // 获取地图文件的父目录（相对于可执行文件） "assets/maps/level1.tmj" -> "assets/maps"
        //     auto map_dir = std::filesystem::path(this->mapPath_).parent_path();
        //     // 合并路径（相对于可执行文件）并返回。 /* std::filesystem::canonical：解析路径中的当前目录（.）和上级目录（..）导航符，
        //                                       /*  得到一个干净的路径 */
        //     auto final_path = std::filesystem::canonical(map_dir / imagePath);
        //     return final_path.string();
        // }
        // catch (const std::exception& e) {
        //     log::Error("[{}]解析路径失败: {}", this->GetClassName(), e.what());
        //     return imagePath;
        // }

        std::filesystem::path mapDir = std::filesystem::path(this->mapPath_).parent_path();
        std::filesystem::path full = mapDir / imagePath;
        // 仅做词法规范化，不访问磁盘
        full = full.lexically_normal();
        return full.string();
    }
} // namespace engine::scene