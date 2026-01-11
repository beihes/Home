#include "../inc/Student.h"

namespace app::school {
    Student::Student(const std::string& id, const std::string& name)
        : studentId_(id), name_(name) {
        
    }

    void Student::enroll(Course* course) {
        courses_.push_back(course);
    }

    void Student::setScore(Exam* exam, int score) {
        scores_[exam] = score;
    }
}

