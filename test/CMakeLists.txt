# CMakeList.txt: Engine_Test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

set(CMAKE_PROJECT_NAME test)

file(GLOB_RECURSE UTILS_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE UTILS_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} STATIC ${UTILS_INCLUDE} ${UTILS_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(cJSON CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    cjson cjson_utils 
    CURL::libcurl EnTT::EnTT fmt::fmt imgui::imgui
    SDL3::SDL3)