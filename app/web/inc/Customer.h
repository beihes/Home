#pragma once
#include <iostream>
#include <string>
#include "StuSchedule.h"
#include <nlohmann/json.hpp>
#include "utils/log/inc/Log.h"

typedef void CURL;
using namespace utils;

class Url
{
public:
    Url();
    ~Url();
public:
    void SetUrl(const char* midUrl);
    void ResetCurl();
	void InsertHeader(const char* first, const char* second);
    void ResetHeader();
	void ResetJson() { jsonData.clear(); };
	void Perform();
public:
    void Print() {
        log::Debug("httpCode:\t{}", this->httpCode);
        log::Debug("urlData:\t{}", this->urlData);
	};
public:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
public:
	nlohmann::json jsonData;
	std::string url = "";
	CURL* curl = nullptr;
	struct curl_slist* header = nullptr;
	std::string urlData = "";
	long int httpCode = 0;
private:
};

class Customer
{
public:
	Customer() {};
	Customer(const std::string name, const std::string password) :name(name), password(password) {};
	~Customer() {};
public:
	char* GetName() { return (char*)name.c_str(); };
	char* GetPassword() { return (char*)password.c_str(); };
public:
	void SetSchedule(StuSchedule midSchedule) { schedule = midSchedule; };
	void SetSchedule(const char* midStr) { schedule.SetSemester(midStr); };
	void SetData();
public:
	std::string name = "";
	std::string password = "";
	std::string semester = "";
	StuSchedule schedule;
private:
};