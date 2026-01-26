#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <cJSON/cJSON.h>
#include <cJSON/cJSON_Utils.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <string_view>
#include "JsonView.h"
#include "JsonViewConst.h"
#include <iostream>

#define JSON_PRINT(expr) (expr).print(#expr)

namespace utils::json {
    /* @brief cJSON初始化函数 */
    void Init() noexcept;

    class Json {
    public:
        Json();
        Json(const char* str);
        // explicit Json(cJSON* root) noexcept :data_(root) {};
        ~Json() noexcept;
    public:
        /// @brief 不允许拷贝
        Json(const Json&) = delete;
        Json& operator=(const Json&) = delete;
        /// @brief 允许移动
        Json(Json&&) noexcept;
        Json& operator=(Json&&) noexcept;
    public:
        bool clear();
        bool empty()noexcept { return this->data_ == nullptr; }
        bool parse(const char* jsonStr) noexcept;
        bool parse(const std::string& jsonStr) noexcept;
        void reset() noexcept;
    public:
        /*  文件读取 */
        bool read(const std::string& filePath);
        /* 文件写入 */
        bool write(const std::string& filePath);
    public:
        /* 输出为 JSON 字符串 */
        std::string dump(bool formatted = false) const;
        /* 直接打印（调试） */
        void print(bool formatted = false) const;
    public:
        /// @brief 不允许将指针暴露出来 
        // cJSON* get() { return this->data_; }
    public:
        /// @brief 根节点视图
        JsonView root() noexcept;
        JsonViewConst root() const noexcept;
    public:
        JsonView operator[](const char* key) noexcept;
        JsonView operator[](const std::string& key) noexcept;
        JsonView operator[](const int key) noexcept;
        JsonViewConst operator[](const char* key) const noexcept;
        JsonViewConst operator[](const std::string& key) const noexcept;
        JsonViewConst operator[](const int key) const noexcept;
    public:
        // Json& operator=(const char* str)noexcept;
        Json operator=(const char* str) const noexcept;
        explicit operator bool() const noexcept { return data_ != nullptr; }
    private:
        
    private:
        cJSON* data_ = nullptr;
    };

    void TestJsonAllFeatures_Log();
}