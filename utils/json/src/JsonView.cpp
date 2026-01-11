#include "../inc/JsonView.h"

namespace utils::json
{
    JsonView::JsonView(cJSON* self) noexcept : self_(self) {

    }

    JsonView::JsonView(cJSON* parent, const char* key) noexcept :parent_(parent) {
        if (!parent || !key)return;
        this->endParent_ = this->parent_;
        if (cJSON_IsObject(parent)) {
            this->self_ = cJSON_GetObjectItemCaseSensitive(parent, key);
            this->key_.push_back(key);
        }
    }

    JsonView::JsonView(cJSON* parent, const std::string& key) noexcept :parent_(parent) {
        if (!parent)return;
        this->endParent_ = this->parent_;
        if (cJSON_IsObject(parent)) {
            this->self_ = cJSON_GetObjectItemCaseSensitive(parent, key.c_str());
            this->key_.push_back(key);
        }
    }

    JsonView::JsonView(cJSON* parent, const int key) noexcept :parent_(parent) {
        if (!parent) return;
        this->endParent_ = this->parent_;
        if (!cJSON_IsArray(parent)) return;
        if (key < 0) return;
        this->self_ = cJSON_GetArrayItem(parent, key);
        this->key_.push_back(key);
    }

    JsonView::JsonView(cJSON* endParent, std::vector<JsonKey> key) noexcept {
        this->key_ = std::move(key);
        this->endParent_ = endParent;
    }

    JsonView::~JsonView() noexcept {

    }

    JsonView JsonView::get(const char* key) const noexcept {
        if (!cJSON_IsObject(this->self_)) return {};
        return JsonView(cJSON_GetObjectItemCaseSensitive(this->self_, key));
    }

    JsonView JsonView::at(int index) const noexcept {
        if (!cJSON_IsArray(this->self_)) return {};
        return JsonView(cJSON_GetArrayItem(this->self_, index));
    }

    int JsonView::size() const noexcept {
        return cJSON_IsArray(this->self_) ? cJSON_GetArraySize(this->self_) : 0;
    }

    // ---------- push_back ----------
    void JsonView::push_back(int value) {
        if (!this->isArray()) return;
        cJSON_AddItemToArray(this->self_, cJSON_CreateNumber(value));
    }

    void JsonView::push_back(double value) {
        if (!this->isArray()) return;
        cJSON_AddItemToArray(this->self_, cJSON_CreateNumber(value));
    }

    void JsonView::push_back(const char* value) {
        if (!this->isArray()) return;
        cJSON_AddItemToArray(this->self_, cJSON_CreateString(value));
    }

    void JsonView::push_back(const std::string& value) {
        this->push_back(value.c_str());
    }

    void JsonView::push_back(bool value) {
        if (!this->isArray()) return;
        cJSON_AddItemToArray(this->self_, cJSON_CreateBool(value));
    }

    JsonView& JsonView::operator=(const char* value) {
        if (!this->endParent_)return *this;
        cJSON* newItem = cJSON_CreateString(value);
        if (this->self_) {
            /* 替换原有内容 */
            this->replace(newItem);
        } else {
            /* 增加新内容 */
            this->add(newItem);
        }
        return *this;
    }

    JsonView& JsonView::operator=(const std::string& value) {
        return (*this = value.c_str());
    }

    JsonView& JsonView::operator=(int value) {
        if (!this->endParent_)return *this;
        cJSON* newItem = cJSON_CreateNumber(value);
        if (this->self_) {
            /* 替换原有内容 */
            this->replace(newItem);
        } else {
            /* 增加新内容 */
            this->add(newItem);
        }
        return *this;
    }

    JsonView& JsonView::operator=(double value) {
        if (!this->endParent_)return *this;
        cJSON* newItem = cJSON_CreateNumber(value);
        if (this->self_) {
            /* 替换原有内容 */
            this->replace(newItem);
        } else {
            /* 增加新内容 */
            this->add(newItem);
        }
        return *this;
    }

    JsonView& JsonView::operator=(bool value) {
        if (!this->endParent_)return *this;
        cJSON* newItem = cJSON_CreateBool(value);
        if (this->self_) {
            /* 替换原有内容 */
            this->replace(newItem);
        } else {
            /* 增加新内容 */
            this->add(newItem);
        }
        return *this;
    }

    JsonView JsonView::operator[](const char* key) noexcept {
        this->key_.push_back(key);
        if (!this->self_ || !cJSON_IsObject(this->self_)) {
            // this->key_.push_back(key);
            return JsonView(this->endParent_, this->key_);
        }
        return JsonView(this->self_, key);
    }

    JsonView JsonView::operator[](const std::string& key) noexcept {
        return this->operator[](key.c_str());
    }

    // ---------- operator[](int) ----------
    JsonView JsonView::operator[](int key) noexcept {
        this->key_.push_back(key);
        if (!this->self_ || !cJSON_IsArray(this->self_)) {
            return JsonView(this->endParent_, this->key_);
        }
        return JsonView(this->self_, key);
    }

    JsonView& JsonView::add(cJSON* item) {
        cJSON* current = endParent_;
        for (auto it = this->key_.begin(); it != this->key_.end(); ++it) {
            bool isLast = (it == this->key_.end() - 1);
            /* ---------- object key ---------- */
            if (std::holds_alternative<std::string>(*it)) {
                const std::string& key = std::get<std::string>(*it);
                /* 强制类型切换为 object */
                if (!cJSON_IsObject(current)) {
                    cJSON* obj = cJSON_CreateObject();
                    this->replace(obj);
                    current = obj;
                }
                if (isLast) {
                    cJSON* exist = cJSON_GetObjectItem(current, key.c_str());
                    if (exist)
                        cJSON_ReplaceItemInObject(current, key.c_str(), item);
                    else
                        cJSON_AddItemToObject(current, key.c_str(), item);
                    return *this;
                }
                /* 决定下一个节点类型 */
                cJSON* nextNode = nullptr;
                if (std::holds_alternative<int>(*(it + 1)))
                    nextNode = cJSON_CreateArray();
                else
                    nextNode = cJSON_CreateObject();
                cJSON* exist = cJSON_GetObjectItem(current, key.c_str());
                if (exist) {
                    cJSON_ReplaceItemInObject(current, key.c_str(), nextNode);
                } else {
                    cJSON_AddItemToObject(current, key.c_str(), nextNode);
                }
                current = nextNode;
            }
            /* ---------- array index ---------- */
            else if (std::holds_alternative<int>(*it)) {
                int index = std::get<int>(*it);
                /* 强制类型切换为 array */
                if (!cJSON_IsArray(current)) {
                    cJSON* arr = cJSON_CreateArray();
                    this->replace(arr);
                    current = arr;
                }
                /* array 扩容 */
                while (cJSON_GetArraySize(current) <= index)
                    cJSON_AddItemToArray(current, cJSON_CreateNull());
                if (isLast) {
                    cJSON_ReplaceItemInArray(current, index, item);
                    return *this;
                }
                /* 取 child */
                cJSON* child = cJSON_GetArrayItem(current, index);
                if (!child || (!cJSON_IsObject(child) && !cJSON_IsArray(child))) {
                    /* 根据下一个 key 类型创建 object/array */
                    if (std::holds_alternative<int>(*(it + 1)))
                        child = cJSON_CreateArray();
                    else
                        child = cJSON_CreateObject();
                    cJSON_ReplaceItemInArray(current, index, child);
                }
                current = child;
            }
        }
        return *this;
    }

    void JsonView::replace(cJSON* item) {
        // self_ 只表示“直接赋值”，但仍要区分 object / array
        if (std::holds_alternative<std::string>(this->key_[0])) {
            cJSON_ReplaceItemInObject(this->endParent_, std::get<std::string>(this->key_[0]).c_str(), item);
        } else {
            cJSON_ReplaceItemInArray(this->endParent_, std::get<int>(this->key_[0]), item);
        }
    }
}
