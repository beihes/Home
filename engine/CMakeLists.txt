# CMakeList.txt: Engine_Test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

set(CMAKE_PROJECT_NAME engine)

file(GLOB_RECURSE ENGINE_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE ENGINE_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} STATIC ${ENGINE_INCLUDE} ${ENGINE_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(box2d CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    utils
    box2d::box2d EnTT::EnTT imgui::imgui
    SDL3::SDL3 $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static> SDL3_ttf::SDL3_ttf)

# TODO: 如有需要，请添加测试并安装目标。
