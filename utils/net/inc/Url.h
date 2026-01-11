#pragma once

#include <string>
#include <memory>

typedef void CURL;
struct curl_slist;

namespace utils::net {
    class Url {
    public:
        Url();
        Url(const std::string& website);
        ~Url();
    public:
        /* 不允许拷贝 */
        Url(const Url&) = delete;
        Url& operator=(const Url&) = delete;
        /* 允许移动 */
        Url(Url&& other) noexcept;
        Url& operator=(Url&& other) noexcept;
    public:
        void Insert_Header(const std::string& first, const std::string& second);/* 插入请求头 */
    public:
        void Set_Website(const std::string& website);    /* 设置目标网址 */
        void Set_TxData(const std::string& txData);     /* 设置发送数据 */
    public:
        void Reset_Url();       /* 重置 url 指针 */
        void Reset_Header();    /* 重置请求头 */
        void Clear_Website();   /* 重置目标网址 */
        void Clear_RxData();    /* 重置接收数据 */
        void Clear_TxData();    /* 重置发送数据 */
        void Reset();           /* 重置所有资源，不包括内存 */
    public:
        void Init();
        void Clear();   /* 清空所有资源，包括内存 */
    public:
        void Perform();
    public:
        const std::string& Get_Website() const noexcept { return this->website_; }
        const std::string& Get_RxData() const noexcept { return this->rxData_; }
        const std::string& Get_TxData() const noexcept { return this->txData_; }
        int Get_HttpCode() const noexcept { return this->httpCode_; }
    public:
        void Print();
    private:
        static size_t RxCallback(void* contents, size_t size, size_t nmemb, void* userp);
    private:
        CURL* url_;                             /// @brief curl指针
        curl_slist* header_ = nullptr;          /// @brief 标头
        std::string website_;                   /// @brief 网址
        std::string rxData_;                    /// 接收的数据
        std::string txData_;                    /// 发送的数据
        int httpCode_;                          /// 状态代码
    };
}