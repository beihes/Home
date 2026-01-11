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
    class Course;
    class Teacher {
    public:
        Teacher() = default;
        Teacher(const std::string& id, const std::string& name);
        ~Teacher() = default;
    public:
        void addCourse(Course* course);
        const std::string& getName() const;
    private:
        std::string teacherId_;
        std::string name_;
        std::vector<Course*> courses_;
    };
}