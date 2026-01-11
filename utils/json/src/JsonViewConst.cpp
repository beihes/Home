#include "../inc/JsonViewConst.h"

namespace utils::json {
    JsonViewConst::JsonViewConst(const cJSON* self) noexcept : self_(self) {

    }

    JsonViewConst::JsonViewConst(const cJSON* parent, const char* key) noexcept {
        this->self_ = cJSON_GetObjectItemCaseSensitive(parent, key);
    }
    JsonViewConst::JsonViewConst(const cJSON* parent, const std::string& key) noexcept {
        this->self_ = cJSON_GetObjectItemCaseSensitive(parent, key.c_str());
    }

    JsonViewConst::JsonViewConst(const cJSON* parent, const int key) noexcept {
        this->self_ = cJSON_GetArrayItem(parent, key);
    }

    int JsonViewConst::size() const noexcept {
        return cJSON_IsArray(this->self_) ? cJSON_GetArraySize(this->self_) : 0;
    }

    JsonViewConst JsonViewConst::operator[](const char* key) noexcept {
        if (!this->self_ || !cJSON_IsObject(this->self_)) { return JsonViewConst(); }
        return JsonViewConst(this->self_, key);
    }

    JsonViewConst JsonViewConst::operator[](const std::string& key) noexcept {
        return this->operator[](key.c_str());
    }

    JsonViewConst JsonViewConst::operator[](const int index) noexcept {
        if (!this->self_ || !cJSON_IsArray(this->self_)) { return JsonViewConst{}; }
        return JsonViewConst(this->self_, index);
    }
}