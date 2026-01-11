# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.20)

# 如果支持，请为 MSVC 编译器启用热重载。
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Home")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(app)
add_subdirectory(engine)
add_subdirectory(utils)
add_subdirectory(main)

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        _DEBUG
    )
endif()