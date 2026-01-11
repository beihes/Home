#include "../inc/Schedule.h"

namespace app::school {
    void Schedule::addCourse(utils::time::Week day, Course* course) {
        table_[day].push_back(course);
    }

    const std::vector<Course*>& Schedule::getCourses(utils::time::Week day) const {
        static std::vector<Course*> empty;
        auto it = table_.find(day);
        return it != table_.end() ? it->second : empty;
    }
}

