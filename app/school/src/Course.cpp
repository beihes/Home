#include "../inc/Course.h"
#include "../inc/Teacher.h"

namespace app::school {
    Course::Course(const std::string& id, const std::string& name, int credit, Teacher* teacher)
        : courseId_(id), name_(name), credit_(credit), teacher_(teacher) {
        teacher_->addCourse(this);
    }

    void Course::addExam(Exam* exam) {
        exams_.push_back(exam);
    }

    const std::string& Course::getName() const {
        return name_;
    }
}


