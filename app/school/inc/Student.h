#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <vector>
#include <unordered_map>

namespace app::school {
    class Course;
    class Exam;
    class Student {
    public:
        Student() = default;
        Student(const std::string& studentNumber, const std::string& password);
        ~Student() = default;
    public:
        void enroll(Course* course);
        void setScore(Exam* exam, int score);
        void GetData_FormWeb();
    private:
        std::string studentNumber_;
        std::string password_;
        std::string name_;
        std::string term_;
        std::string userId_;
        std::string token_;
        std::vector<Course*> courses_;
        std::unordered_map<Exam*, int> scores_;
    };
}