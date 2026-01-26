#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <unordered_map>
#include "utils/json/inc/Json.h"

namespace app::school {
    class Snake {
    public:
        Snake() = default;
        ~Snake() = default;
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Snake(const Snake&) = delete;
        Snake& operator=(const Snake&) = delete;
        Snake(Snake&&) = delete;
        Snake& operator=(Snake&&) = delete;
    public:
        static utils::json::Json loginSSO(const std::string& username, const std::string& password);
        static utils::json::Json loginVPN(const std::string& username, const std::string& password);
        static utils::json::Json loginWeChatApp(const std::string& username, const std::string& password);
    public:
        static utils::json::Json GetKB_By_WeChatApp(const utils::json::Json& sendData,const std::string& token);
    private:
        static std::string rootSSOLogin;
        static std::string rootVPNURL;
        static std::string wechatAppAPI;
        static const std::unordered_map<std::string, std::string> headers;
    };
}