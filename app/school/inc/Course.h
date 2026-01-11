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

namespace app::school {
    class Teacher;
    class Exam;

    struct TimeSlot {
        int day;        // 1~7
        int start;      // 分钟，如 9:00 = 540
        int end;        // 结束分钟

        bool conflictWith(const TimeSlot& other) const {
            return day == other.day &&
                !(end <= other.start || start >= other.end);
        }
    };

    class Course {
    public:
        Course() = default;
        Course(const std::string& id, const std::string& name, int credit, Teacher* teacher);
        ~Course() = default;
    public:
        void addExam(Exam* exam);
        const std::string& getName() const;
    private:
        std::string courseId_;
        std::string name_;
        int credit_;
        Teacher* teacher_;
        std::vector<Exam*> exams_;
    };
}