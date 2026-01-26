#include "../inc/Snake.h"
#include "utils/log/inc/Log.h"
#include "utils/net/inc/Url.h"

using namespace utils;

namespace app::school {
    std::string Snake::rootSSOLogin = "https://sso.cczu.edu.cn/cas/login";
    std::string Snake::rootVPNURL = "https://zmvpn.cczu.edu.cn";
    std::string Snake::wechatAppAPI = "http://jwqywx.cczu.edu.cn";
    const std::unordered_map<std::string, std::string> Snake::headers = {
       {"content-type", "application/json;charset=UTF-8"},
       {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36"}
    };

    utils::json::Json Snake::loginSSO(const std::string& username, const std::string& password) {
        utils::net::Url url;
        url.Set_Website(Snake::rootSSOLogin);
        url.Insert_Header(Snake::headers);
        std::string postData = "username=" + username + "&password=" + password;
        url.Set_TxData(postData);
        url.Perform();
        std::string response = url.Get_RxData();
        utils::json::Json jsonResponse;
        if (jsonResponse.parse(response)) {
            return jsonResponse;
        }
        return utils::json::Json();
    }

    utils::json::Json Snake::loginVPN(const std::string& username, const std::string& password) {
        utils::net::Url url;
        url.Set_Website(Snake::rootVPNURL + "/api/login");
        url.Insert_Header(Snake::headers);
        utils::json::Json postData;
        postData["username"] = username;
        postData["password"] = password;
        url.Set_TxData(postData.dump());
        url.Perform();
        std::string response = url.Get_RxData();
        utils::json::Json jsonResponse;
        if (jsonResponse.parse(response)) {
            return jsonResponse;
        }
        return utils::json::Json();
    }

    utils::json::Json Snake::loginWeChatApp(const std::string& username, const std::string& password) {
        utils::net::Url url;
        url.Set_Website(Snake::wechatAppAPI + ":8180/api/login");
        url.Insert_Header(Snake::headers);
        utils::json::Json postData;
        postData["userid"] = username;
        postData["userpwd"] = password;
        url.Set_TxData(postData.dump());
        url.Perform();
        std::string response = url.Get_RxData();
        utils::json::Json jsonResponse;
        if (jsonResponse.parse(response)) {
            return jsonResponse;
        }
        return utils::json::Json();
    }

    utils::json::Json Snake::GetKB_By_WeChatApp(const utils::json::Json& sendData,const std::string& token) {
        utils::net::Url url;
        url.Set_Website(Snake::wechatAppAPI + ":8180/api/kb_xq_xh");
        url.Insert_Header(Snake::headers);
        url.Insert_Header("Authorization", "Bearer " + token);
        url.Set_TxData(sendData.dump());
        url.Perform();
        std::string response = url.Get_RxData();
        utils::json::Json jsonResponse;
        if (jsonResponse.parse(response)) {
            return jsonResponse;
        }
        return utils::json::Json();
    }
}