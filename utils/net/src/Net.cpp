#include "../inc/Net.h"
#include "../inc/Url.h"
#include "utils/log/inc/Log.h"
#include "json/inc/Json.h"
#include <curl/curl.h>


namespace utils::net {
    /* net初始化状态 */
    static bool initState = false;
    bool Init() {
        if (initState) return true;
        CURLcode code = curl_global_init_mem(CURL_GLOBAL_DEFAULT, SDL_malloc, SDL_free, SDL_realloc, SDL_strdup, SDL_calloc);
        initState = true;
        log::Trace("net模块初始化完成");
        return code == CURLE_OK;
    }

    void Close() {
        if (!initState) return;
        curl_global_cleanup();
        log::Trace("net模块清理完成");
    }

    bool Test() {
        /* 1. 初始化 curl（必须在任何 Url 使用前） */
        if (!Init()) {
            log::Error("curl init failed");
            return false;
        }

        /* 2. 创建 Url 对象 */
        Url url;

        /* 3. 设置目标网址（示例：httpbin，用于测试） */
        url.Set_Website("http://httpbin.org/post");

        /* 4. 设置请求头 */
        url.Insert_Header("Content-Type", "application/json");
        url.Insert_Header("Accept", "application/json");

        json::Json midJson;
        midJson["name"] = "test_user";
        midJson["id"] = 12345.1;
        midJson.print();
        nlohmann::json J;
        J["arr"][1] = 1;
        // J["11"] = 0;
        log::Info("nlohmann:{}", J.dump());

        url.Set_TxData(midJson.dump());

        /* 6. 发起请求 */
        url.Perform();

        /* 7. 输出结果 */
        url.Print();
        /* 8. 全局清理 */
        Close();
        return true;
    }
}