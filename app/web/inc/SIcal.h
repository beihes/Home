// #pragma once
// #include <libical/ical.h>
// #include <iostream>
// #include <string>

// void Ical_Test()
// {
//     // 创建一个新的日历组件
//     icalcomponent *calendar = icalcomponent_new(ICAL_VCALENDAR_COMPONENT);
//     icalcomponent_add_property(calendar, icalproperty_new_version("2.0"));
//     icalcomponent_add_property(calendar, icalproperty_new_prodid("-//MyApp//EN"));

//     // 示例课程信息
//     std::string course_name = "集成电路芯片制造技术";
//     std::string location = "X立德楼318";
//     std::string start_time_str = "20250414T080000Z"; // 2025年4月14日 08:00 UTC
//     std::string end_time_str = "20250414T093000Z";   // 2025年4月14日 09:30 UTC

//     // 创建一个新的事件组件
//     icalcomponent *event = icalcomponent_new(ICAL_VEVENT_COMPONENT);
//     icalcomponent_add_property(event, icalproperty_new_dtstart(icaltime_from_string(start_time_str.c_str())));
//     icalcomponent_add_property(event, icalproperty_new_dtend(icaltime_from_string(end_time_str.c_str())));
//     icalcomponent_add_property(event, icalproperty_new_summary(course_name.c_str()));
//     icalcomponent_add_property(event, icalproperty_new_location(location.c_str()));

//     // 将事件添加到日历中
//     icalcomponent_add_component(calendar, event);

//     // 将日历导出为字符串并打印
//     const char *ical_str = icalcomponent_as_ical_string(calendar);
//     std::cout << ical_str << std::endl;

//     // 清理资源
//     icalcomponent_free(calendar);
// }