#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <map>
#include <vector>
#include <string>
#include "utils/time/inc/Time.h"

namespace app::school {
    class Course;
    class Schedule {
    public:
        Schedule() = default;
        ~Schedule() = default;
        public:
    void addCourse(utils::time::Week day, Course* course);
    const std::vector<Course*>& getCourses(utils::time::Week day) const;
    private:
        std::map<utils::time::Week, std::vector<Course*>> table_;
    };
}