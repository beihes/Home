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

namespace utils::json {
    class JsonViewConst {
    public:
        JsonViewConst() noexcept = default;
        JsonViewConst(const cJSON* self) noexcept;
        JsonViewConst(const cJSON* parent, const char* key) noexcept;
        JsonViewConst(const cJSON* parent, const std::string& key) noexcept;
        JsonViewConst(const cJSON* parent, const int key) noexcept;
    public:
        /* ---------- 类型判断 ---------- */
        bool isObject() const noexcept { return cJSON_IsObject(this->self_); }
        bool isArray()  const noexcept { return cJSON_IsArray(this->self_); }
        bool isString() const noexcept { return cJSON_IsString(this->self_); }
        bool isNumber() const noexcept { return cJSON_IsNumber(this->self_); }
        bool isBool()   const noexcept { return cJSON_IsBool(this->self_); }
        /* ---------- 值读取 ---------- */
        template <typename T>
        T value(T def) const noexcept {
            if (!self_) return def;
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
        int size() const noexcept;
    public:
        JsonViewConst operator[](const char* key) noexcept;              /* 链式访问子对象 */
        JsonViewConst operator[](const std::string& key) noexcept;       /* 链式访问子对象 */
        JsonViewConst operator[](const int index) noexcept;              /* 访问数组元素 */
    public:
        explicit operator bool() const noexcept { return this->self_ != nullptr; }
    private:
        mutable const cJSON* self_ = nullptr;
    };
}