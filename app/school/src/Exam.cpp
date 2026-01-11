#include "../inc/Exam.h"
#include "../inc/Course.h"

namespace app::school {
    Exam::Exam(Course* course, const std::string& location, std::chrono::system_clock::time_point time)
        : course_(course), location_(location), time_(time) {
        
    }
    
    Course* Exam::getCourse() const {
        return course_;
    }
}

