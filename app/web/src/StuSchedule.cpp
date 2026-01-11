#include "../inc/StuSchedule.h"

inline void StuSchedule::Print()
{
#ifdef _DEBUG
	std::cout << "Class:\t" << userClass << "\tSemester:\t" << semester << std::endl;
	std::cout << "Name:\t" << userName << "\tStudentId:\t" << studentId << std::endl;
	std::cout << "\t" << "星期一" << "\t" << "星期二" << "\t" << "星期三" << "\t" << "星期四" << "\t" << "星期五" << "\t" << "星期六" << "\t" << "星期日" << std::endl;
	for (int i = 0; i < lessonNum; i++)
	{
		std::string midStr = "第" + std::to_string(i + 1) + "节";
		std::cout << midStr << "\t";
		for (int j = 0; j < WEEKDAY; j++)
		{
			int midNum = 0;
			for (auto v : lesson[i][j])
			{
				midNum++;
			}
			if (midNum != 0)
			{
				std::cout << midNum;
			}
			std::cout << "\t";
		}
		std::cout << std::endl;
	}
#endif // _DEBUG
}

void StuSchedule::GetDataToLesson(const std::string& data)
{
	nlohmann::json midJson = nlohmann::json::parse(data);
	// 检查是否包含 "message" 键
	if (midJson.contains("message") && midJson["message"].is_array()) {
		for (const auto& item : midJson["message"]) {
			int jc = item["jc"].get<int>();
			std::string xq = item["xq"].get<std::string>();

			for (int i = 0; i < StuSchedule::WEEKDAY; i++)
			{
				std::string kc = "kc" + std::to_string(i + 1);
				std::string midKc = item[kc].get<std::string>();
				if (midKc == "")
				{
					continue;
				}
				// 正则表达式，确保课程名称可以包含中文、英文、数字、空格
				std::regex pattern(R"(([^"\s]+(?:\s*[^"\s,，/])*)\s+(\S+)\s+(\d+-\d+(?:,\d+-\d+)*),/)");
				std::smatch match;
				auto begin = std::sregex_iterator(midKc.begin(), midKc.end(), pattern);
				auto end = std::sregex_iterator();
				for (auto it = begin; it != end; ++it) {
					StuLesson midStuLesson;
					midStuLesson.SetName((*it)[1].str().c_str());
					midStuLesson.SetClassRoom((*it)[2].str().c_str());
					midStuLesson.SetWeeks((*it)[3].str().c_str());
					midStuLesson.SetWeekDay(i);
					this->SetLesson(jc, i, midStuLesson);
				}
			}
			if (this->course[0].name != "")
			{
				continue;
			}
			for (int i = 0; i < MAX_COURSE_NUM; i++)
			{
				CourseData midCourseData;
				std::string midStr = "kcmc" + std::to_string(i + 1);
				auto midKcmc = item[midStr];
				if (midKcmc.is_null())
				{
					break;
				}
				std::string midName = item[midStr].get<std::string>();
				midCourseData.SetName(midName.c_str());
				midStr.clear();
				midStr = "lbdh" + std::to_string(i + 1);
				midCourseData.SetGrade(item[midStr].get<std::string>().c_str());
				midStr.clear();
				midStr = "xs" + std::to_string(i + 1);
				midCourseData.SetTime(std::stof(item[midStr].get<std::string>()));
				midStr.clear();
				midStr = "xf" + std::to_string(i + 1);
				midCourseData.SetScore(std::stof(item[midStr].get<std::string>()));
				midStr.clear();
				midStr = "xklb" + std::to_string(i + 1);
				midCourseData.SetState(item[midStr].get<std::string>().c_str());
				midCourseData.Print();
				this->SetCourseData(i, midCourseData);
			}
			this->userClass = item["bj"].get<std::string>();
			this->userClass = RemoveSpaces(this->userClass);
			this->userName = item["xm"].get<std::string>();
			this->userName = RemoveSpaces(this->userName);
			this->studentId = item["xh"].get<std::string>();
			this->studentId = RemoveSpaces(this->studentId);
		}
	}
	else {
		std::cerr << "message 键不存在或不是数组" << std::endl;
	}
	this->Print();
}