#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <cJSON/cJSON.h>
#include <cJSON/cJSON_Utils.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace utils::json {
    using JsonKey = std::variant<int, std::string>;
    using JsonValue = std::variant<int, double, bool, std::string>;
    class JsonView {
    public:
        JsonView() noexcept = default;
        JsonView(cJSON* self) noexcept;
        JsonView(cJSON* parent, const char* key) noexcept;
        JsonView(cJSON* parent, const std::string& key) noexcept;
        JsonView(cJSON* parent, const int key) noexcept;
        JsonView(cJSON* endParent, std::vector<JsonKey> key) noexcept;
        ~JsonView() noexcept;
    public:
        /* ---------- 类型判断 ---------- */
        bool isObject() const noexcept { return cJSON_IsObject(this->self_); }
        bool isArray()  const noexcept { return cJSON_IsArray(this->self_); }
        bool isString() const noexcept { return cJSON_IsString(this->self_); }
        bool isNumber() const noexcept { return cJSON_IsNumber(this->self_); }
        bool isBool()   const noexcept { return cJSON_IsBool(this->self_); }

        /* ---------- 对象访问 ---------- */
        JsonView get(const char* key) const noexcept;
        JsonView at(int index) const noexcept;

        /* ---------- 数组访问 ---------- */
        void push_back(bool value);
        void push_back(int value);
        void push_back(double value);
        void push_back(const char* value);
        void push_back(const std::string& value);
        int size() const noexcept;

        /* ---------- 值读取 ---------- */
        template <typename T>
        T value(T def) const noexcept {
            if (!this->self_) return def;
            if constexpr (std::is_same_v<T, int>) {
                if (cJSON_IsNumber(self_)) return (int)cJSON_GetNumberValue(this->self_);
            } else if constexpr (std::is_same_v<T, double>) {
                if (cJSON_IsNumber(self_)) return cJSON_GetNumberValue(this->self_);
            } else if constexpr (std::is_same_v<T, bool>) {
                if (cJSON_IsBool(self_)) return cJSON_IsTrue(this->self_);
            } else if constexpr (std::is_same_v<T, std::string>) {
                if (cJSON_IsString(self_)) return std::string(cJSON_GetStringValue(this->self_));
            }
            return def;
        }
    public:
        // 赋值操作符
        JsonView& operator=(const char* value);
        JsonView& operator=(const std::string& value);
        JsonView& operator=(int value);
        JsonView& operator=(double value);
        JsonView& operator=(bool value);
        JsonView operator[](const char* key) noexcept;          /* 链式访问子对象 */
        JsonView operator[](const std::string& key) noexcept;   /* 链式访问子对象 */
        JsonView operator[](int key) noexcept;                  /* 访问数组元素 */
    public:
        explicit operator bool() const noexcept { return this->self_ != nullptr; }
    private:
        JsonView& add(cJSON* item);
        void replace(cJSON* item);
    private:
        cJSON* endParent_ = nullptr;    /* 最后一个有key值的节点 */
        cJSON* parent_ = nullptr;
        cJSON* self_ = nullptr;
        std::vector<JsonKey> key_;      /* 未找到的key存储区 */
    };
}