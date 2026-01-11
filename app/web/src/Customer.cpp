#include "../inc/Customer.h"
#include <curl/curl.h>

Url::Url() {
    curl = curl_easy_init();
};
Url::~Url() {
    if (curl)curl_easy_cleanup(curl);
    if (header)curl_slist_free_all(header);
};
void Url::SetUrl(const char* midUrl) {
    url.clear(); url = midUrl;
};
void Url::ResetCurl() {
    if (curl)curl_easy_cleanup(curl);
    this->curl = curl_easy_init();
};
inline void Url::InsertHeader(const char* first, const char* second)
{
    std::string midFirstStr = first;
    std::string midSecondStr = second;
    std::string midStr = midFirstStr + ": " + midSecondStr;
    header = curl_slist_append(header, midStr.c_str());
};
void Url::ResetHeader() {
    if (header)curl_slist_free_all(header);
    header = nullptr;
}



void Url::Perform()
{
    urlData.clear();
    if (curl == "")
    {
        return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (header == nullptr)
    {
        return;
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    if (jsonData.empty() == true)
    {
        return;
    }
    std::string midStr = jsonData.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, midStr.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Url::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &urlData);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (res != CURLE_OK)
        std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    ResetCurl();
}

size_t Url::WriteCallback(void* ptr, size_t size, size_t nmemb, void* userStr)
{
    size_t totalSize = size * nmemb;
    std::string* midStr = (std::string*)userStr;
    for (size_t i = 0; i < totalSize; i++)
    {
        midStr->push_back(((char*)ptr)[i]);
    }
    return totalSize;
}

void Customer::SetData()
{
    Url midUrl;
    midUrl.SetUrl("http://jwqywx.cczu.edu.cn:8180/api/login");
    midUrl.InsertHeader("content-type","application/json;charset=UTF-8");
    midUrl.InsertHeader("Accept-Language", "zh-CN,zh;q=0.9,en;q=0.8");
    midUrl.InsertHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
    // midUrl.InsertHeader("Origin", "http://jwqywx.cczu.edu.cn");
    // midUrl.InsertHeader("Referer", "http://jwqywx.cczu.edu.cn/");
    midUrl.InsertHeader("Accept", "application/json, text/plain, */*");
    midUrl.jsonData["userid"] = this->name;
    midUrl.jsonData["userpwd"] = this->password;
    midUrl.Perform();
    midUrl.Print();
    nlohmann::json midJson = nlohmann::json::parse(midUrl.urlData);
    if (midJson.contains("message") && midJson["message"].is_array())
    {
        for (const auto& item : midJson["message"])
        {
            this->semester = item["xq"].get<std::string>();
        }
    }
    std::string yhid = midJson["message"][0]["yhid"].get<std::string>();
    std::string token = "Bearer " + midJson["token"].get<std::string>();
    midUrl.SetUrl("http://jwqywx.cczu.edu.cn:8180/api/kb_xq_xh");
    midUrl.InsertHeader("Authorization", token.c_str());
    midUrl.ResetJson();
    midUrl.jsonData["xh"] = this->name;
    midUrl.jsonData["xq"] = this->schedule.semester;
    midUrl.jsonData["yhid"] = yhid;
    midUrl.Perform();
    midUrl.Print();
    midJson.clear();
    midUrl.ResetCurl();
    if (this->schedule.semester == "")
    {
        this->schedule.semester = this->semester;
    }
     this->schedule.GetDataToLesson(midUrl.urlData);
}