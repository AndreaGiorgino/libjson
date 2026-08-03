option(DEBUG "Enable debug build." OFF)
option(JSON_BUILD_TESTING "Enable testing and build tests." OFF)
option(JSON_BUILD_EXAMPLE "Enable and build example." OFF)

if(DEBUG)
    add_compile_definitions(DEBUG)
    add_compile_options(-g -ggdb -O0)
endif()

if(JSON_BUILD_TESTING)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CTest)
endif()
