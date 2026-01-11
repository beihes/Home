# CMakeList.txt: Home 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
project("main")

find_package(CURL REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

file(GLOB_RECURSE MAIN_INCLUDE
    "inc/*.h"
    "inc/*.hpp"
)

file(GLOB_RECURSE MAIN_SOURCES
    "src/*.cpp"
    "src/*.c"
)

# 将源代码添加到此项目的可执行文件。
add_executable(${CMAKE_PROJECT_NAME} ${MAIN_INCLUDE} ${MAIN_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    app engine utils
    CURL::libcurl EnTT::EnTT fmt::fmt imgui::imgui
    SDL3::SDL3 $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>  SDL3_ttf::SDL3_ttf)