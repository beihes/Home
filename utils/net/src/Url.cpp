#include "../inc/Url.h"
#include "utils/log/inc/Log.h"
#include <curl/curl.h>

namespace utils::net {

    Url::Url() {
        this->url_ = curl_easy_init();
        this->httpCode_ = 0;
    }

    Url::Url(const std::string& website) :website_(website) {
        this->url_ = curl_easy_init();
        this->httpCode_ = 0;
    }

    Url::~Url() {
        this->Clear();
    }

    Url::Url(Url&& other) noexcept
        : url_(other.url_), header_(other.header_)
        , website_(std::move(other.website_)), rxData_(std::move(other.rxData_)), txData_(std::move(other.txData_))
        , httpCode_(other.httpCode_) {
        // 关键：剥夺 other 的资源所有权
        other.url_ = nullptr;
        other.header_ = nullptr;
        other.httpCode_ = 0;
    }

    Url& Url::operator=(Url&& other) noexcept
    {
        if (this == &other) {return *this;}

        // 先释放自身已有资源
        if (this->header_) {
            curl_slist_free_all(this->header_);
            this->header_ = nullptr;
        }
        if (this->url_) {
            curl_easy_cleanup(this->url_);
            this->url_ = nullptr;
        }

        // 接管 other 的资源
        this->url_ = other.url_;
        this->header_ = other.header_;
        this->website_ = std::move(other.website_);
        this->rxData_ = std::move(other.rxData_);
        this->txData_ = std::move(other.txData_);
        this->httpCode_ = other.httpCode_;

        // 清空 other
        other.url_ = nullptr;
        other.header_ = nullptr;
        other.httpCode_ = 0;
        return *this;
    }

    void Url::Insert_Header(const std::string& first, const std::string& second) {
        if (first == "Content-Length") return;
        std::string midStr = first + ": " + second;
        this->header_ = curl_slist_append(this->header_, midStr.c_str());
    }

    void Url::Set_Website(const std::string& website) {
        this->website_.clear();
        this->website_ = website;
    }

    void Url::Set_TxData(const std::string& txData) {
        this->txData_.clear();
        this->txData_ = txData;
    }

    void Url::Reset_Url() {
        if (this->url_) { curl_easy_reset(this->url_); }
    }

    void Url::Reset_Header() {
        if (!this->header_)return;
        curl_slist_free_all(this->header_);
        this->header_ = nullptr;
    }

    void Url::Clear_Website() {
        this->website_.clear();
    }

    void Url::Clear_RxData() {
        this->rxData_.clear();
    }

    void Url::Clear_TxData() {
        this->txData_.clear();
    }

    void Url::Reset() {
        this->Reset_Header();
        this->Clear_RxData();
        this->Clear_TxData();
        this->Reset_Url();
    }

    void Url::Init() {
        if (!this->url_)this->url_ = curl_easy_init();
        this->httpCode_ = 0;
    }

    void Url::Clear() {
        if (this->header_) {
            curl_slist_free_all(this->header_);
            this->header_ = nullptr;
        }
        if (this->url_) {
            curl_easy_cleanup(this->url_);
            this->url_ = nullptr;
        }
        this->website_.clear();
        this->rxData_.clear();
        this->txData_.clear();
        this->httpCode_ = 0;
    }

    void Url::Perform()
    {
        if (!this->url_) {
            log::Error("CURL not initialized");
            return;
        }
        this->rxData_.clear();

        curl_easy_setopt(this->url_, CURLOPT_URL, this->website_.c_str());
        curl_easy_setopt(this->url_, CURLOPT_WRITEFUNCTION, &Url::RxCallback);
        curl_easy_setopt(this->url_, CURLOPT_WRITEDATA, this);

        curl_easy_setopt(this->url_, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(this->url_, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(this->url_, CURLOPT_CONNECTTIMEOUT, 5L);
        curl_easy_setopt(this->url_, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(url_, CURLOPT_HTTPGET, 0L);
        curl_easy_setopt(url_, CURLOPT_POST, 0L);

        if (this->header_) {
            curl_easy_setopt(this->url_, CURLOPT_HTTPHEADER, this->header_);
        }

        if (!this->txData_.empty()) {
            curl_easy_setopt(this->url_, CURLOPT_POST, 1L);
            curl_easy_setopt(this->url_, CURLOPT_POSTFIELDS, this->txData_.c_str());
        } else {
            curl_easy_setopt(this->url_, CURLOPT_HTTPGET, 1L);
        }

        CURLcode res = curl_easy_perform(this->url_);
        curl_easy_getinfo(url_, CURLINFO_RESPONSE_CODE, &this->httpCode_);

        if (res != CURLE_OK) {
            log::Error("curl_easy_perform failed: {}", curl_easy_strerror(res));
        }
        curl_easy_reset(this->url_);
    }

    void Url::Print() {
        log::Debug("httpCode:\t{}", this->httpCode_);
        log::Debug("urlData:\t{}", this->rxData_);
    }

    size_t Url::RxCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t totalSize = size * nmemb;
        auto midUrl = static_cast<Url*>(userp);
        midUrl->rxData_.append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
}