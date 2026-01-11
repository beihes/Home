#include "../inc/Teacher.h"

namespace app::school {
    Teacher::Teacher(const std::string& id, const std::string& name)
        : teacherId_(id), name_(name) {}

    void Teacher::addCourse(Course* course) {
        courses_.push_back(course);
    }

    const std::string& Teacher::getName() const {
        return name_;
    }
}

