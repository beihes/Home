#include "../inc/Student.h"
#include "../inc/Snake.h"
#include "utils/log/inc/Log.h"
#include "utils/net/inc/Net.h"

using namespace utils;

namespace app::school {
    Student::Student(const std::string& studentNumber, const std::string& password)
        : studentNumber_(studentNumber), password_(password) {

    }

    void Student::enroll(Course* course) {
        courses_.push_back(course);
    }

    void Student::setScore(Exam* exam, int score) {
        scores_[exam] = score;
    }

    void Student::GetData_FormWeb() {
        utils::json::Json loginResponse = school::Snake::loginWeChatApp(this->studentNumber_, this->password_);
        this->name_ = loginResponse["message"][0]["yhmc"].value<std::string>("");
        this->userId_ = loginResponse["message"][0]["yhid"].value<std::string>("");
        this->token_ = loginResponse["token"].value<std::string>("");
        this->term_ = loginResponse["message"][0]["xq"].value<std::string>("");
        log::Info("学生 {} 正在从网络获取数据...", this->name_);
        utils::json::Json sendJson;
        sendJson["xh"] = this->studentNumber_;
        sendJson["xq"] = this->term_;
        sendJson["yhid"] = this->userId_;
        utils::json::Json kbRequestData = school::Snake::GetKB_By_WeChatApp(sendJson, this->token_);
        log::Info("学生 {} 课程数据获取完成", this->name_);
        
    }
}

