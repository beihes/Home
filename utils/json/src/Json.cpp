#include "../inc/Json.h"
#include "utils/file/inc/File.h"
#include "utils/log/inc/Log.h"
namespace utils::json
{
    void Init() noexcept {
        cJSON_Hooks hooks;
        hooks.malloc_fn = SDL_malloc;
        hooks.free_fn = SDL_free;
        cJSON_InitHooks(&hooks);
        log::Trace("json模块初始化完成");
    }

    Json::Json() {
        this->data_ = cJSON_CreateObject();
    }

    Json::Json(const char* str) {
        this->parse(str);
    }

    Json::~Json() {
        this->reset();
    }

    Json::Json(Json&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;
    }

    Json& Json::operator=(Json&& other)noexcept {
        if (this != &other) {
            if (this->data_) cJSON_Delete(this->data_);
            this->data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    bool Json::clear() {
        if (!this->data_) { return true; }
        cJSON_Delete(this->data_);
        this->data_ = cJSON_CreateObject();
        return this->data_ == nullptr;
    }

    bool Json::parse(const char* jsonStr) noexcept {
        this->reset();
        this->data_ = cJSON_Parse(jsonStr);
        return this->data_ != nullptr;
    }

    bool Json::parse(const std::string& jsonStr) noexcept {
        return this->parse(jsonStr.c_str());
    }

    void Json::reset() noexcept {
        if (!this->data_) { return; }
        cJSON_Delete(this->data_);
        this->data_ = nullptr;
    }

    bool Json::read(const std::string& filePath) {
        return false;
    }

    bool Json::write(const std::string& filePath) {
        return false;
    }

    std::string Json::dump(bool formatted) const {
        if (!data_) { return ""; }
        char* raw = formatted ? cJSON_Print(data_) : cJSON_PrintUnformatted(data_);
        if (!raw) { return ""; }
        std::string result(raw);
        cJSON_free(raw);
        return result;
    }

    void Json::print(bool formatted) const {
        if (auto str = dump(formatted); !str.empty()) { log::Info("jsonData: {}", str); }
    }

    JsonView Json::root() noexcept {
        return JsonView(this->data_);
    }

    JsonViewConst Json::root() const noexcept {
        return JsonViewConst(this->data_);
    }

    JsonView Json::operator[](const char* key) noexcept {
        if (!data_) { data_ = cJSON_CreateObject(); }
        return JsonView(data_, key);
    }
    JsonView Json::operator[](const std::string& key) noexcept {
        return this->operator[](key.c_str());
    }

    JsonView Json::operator[](const int key) noexcept {
        if (!data_) { data_ = cJSON_CreateObject(); }
        return JsonView(data_, key);
    }

    JsonViewConst Json::operator[](const char* key) const noexcept {
        return JsonViewConst(this->data_, key);
    }

    JsonViewConst Json::operator[](const std::string& key) const noexcept {
        return JsonViewConst(this->data_, key);
    }

    JsonViewConst Json::operator[](const int key) const noexcept {
        return JsonViewConst(this->data_, key);
    }

    Json Json::operator=(const char* str) const noexcept {
        return Json(str);
    }
}