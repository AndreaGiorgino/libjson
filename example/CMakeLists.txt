cmake_minimum_required(VERSION 3.24)

#
# Target
#

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    example
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx
)

set_target_properties(
    example
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_compile_options(
    example
    PRIVATE -Wextra -Wall -pedantic
)

target_include_directories(
    example
    PRIVATE $<TARGET_FILE_DIR:${PROJECT_NAME}>/include
)

target_link_libraries(
    example
    PRIVATE ${PROJECT_NAME}
)
