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
        Student(const std::string& id, const std::string& name);
        ~Student() = default;
    public:
        void enroll(Course* course);
        void setScore(Exam* exam, int score);
    private:
        std::string studentId_;
        std::string name_;
        std::vector<Course*> courses_;
        std::unordered_map<Exam*, int> scores_;
    };
}