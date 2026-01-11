#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <string>
#include <chrono>

namespace app::school {
    class Course;
    class Exam {
    public:
        Exam() = default;
        Exam(Course* course, const std::string& location, std::chrono::system_clock::time_point time);
        ~Exam() = default;
    public:
        Course* getCourse() const;
    private:
        Course* course_;
        std::string location_;
        std::chrono::system_clock::time_point time_;
    };
}