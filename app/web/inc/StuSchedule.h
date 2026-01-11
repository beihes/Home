#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <nlohmann/json.hpp>

// 删除字符串中的所有空格
inline std::string RemoveSpaces(const std::string& str) {
	std::string result = str;
	result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
	return result;
}

class CourseData
{
public:
	CourseData() {};
	~CourseData() {};
public:
	void SetName(const char* midName) { name = midName; };
	void SetTecher(const char* midTeacher) { teacher = midTeacher; };
	void SetTime(float midTime) { time = midTime; };
	void SetScore(float midScore) { score = midScore; };
	void SetGrade(const char* midGrade) { grade = midGrade; };
	void SetState(const char* midState) { state = midState; };
public:
	void Print();
public:
	std::string name = "";		//课程名称
	std::string teacher = "";	//教师
	float time = 0;;			//学时
	float score = 0;			//
	std::string grade = "";		//等级
	std::string state = "";		//课程状态（正常修读）
private:

};

inline void CourseData::Print()
{
#ifdef _DEBUG
	std::cout << "课程名称:\t" << name << std::endl;
	std::cout << "教师:\t" << teacher << std::endl;
	std::cout << "学时:\t" << time << std::endl;
	std::cout << "学分:\t" << score << std::endl;
	std::cout << "等级:\t" << grade << std::endl;
	std::cout << "课程状态:\t" << state << std::endl;
#endif // _DEBUG
}

class StuLesson
{
public:
	StuLesson() {};
	~StuLesson() {};
public:
	void SetName(const char* midName) { name = midName; };
	void SetClassRoom(const char* midClassRoom) { classRoom = midClassRoom; };
	void SetWeeks(const char* midWeeks) { weeks = midWeeks; };
	void SetWeekDay(int midWeekDay) { weekDay = midWeekDay; };
public:
	std::string name = "";		//课程名称
	std::string classRoom = "";	//教室
	std::string weeks = "";		//周次
	int weekDay = 0;			//星期几
private:
};

#define MAX_COURSE_NUM 20

class StuSchedule
{
public:
	StuSchedule() { SetLessonNum(lessonNum); SetCourseDataNum(MAX_COURSE_NUM); };
	StuSchedule(int midLessonNum) { SetLessonNum(midLessonNum); };
	~StuSchedule() {};
public:
	enum WeekDay
	{
		MONDEY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY, WEEKDAY
	};
public:
	void StuData_Init(const char* midUserClass, const char* midUserName, const char* midStudentId)
	{
		userClass = midUserClass; userName = midUserName; studentId = midStudentId;
	}
	void SetSemester(const char* midSemester) { semester = midSemester; };
	//重新设置后需重新设置内容
	void SetLessonNum(int midLessonNum) { lessonNum = midLessonNum; lesson.resize(lessonNum); for (int i = 0; i < lessonNum; i++)lesson[i].resize(WEEKDAY); };
	void SetLesson(int midLessonNum, int weekDay, StuLesson midStuLesson) { lesson[midLessonNum][weekDay].push_back(midStuLesson); };
	void Print();//终端调试用
	void SetCourseDataNum(int midCourseNum) { course.resize(midCourseNum); };
	void SetCourseData(int midCourseNum, CourseData midCourseData) { course[midCourseNum] = midCourseData; };
public:
	//导入的内容为爬取的json数据，还未转化为JSON
	void GetDataToLesson(const std::string& data);
public:
	int lessonNum = 17;
	std::string userClass = "";	//班级
	std::string userName = "";	//姓名
	std::string studentId = "";	//学号
	std::string semester = "";	//学期
	std::vector<std::vector<std::vector<StuLesson>>> lesson;
	std::vector<CourseData> course;
private:
};
