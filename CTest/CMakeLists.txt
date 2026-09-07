cmake_minimum_required(VERSION 3.24)

#
# Target executables
#

set(
    SOURCE_FILES

    # json value
    Test_Ctors_Bool.cxx
    Test_Ctors_Int.cxx
    Test_Ctors_Double.cxx
    Test_Ctors_String.cxx

    # json array
    Test_Ctors_Array.cxx
    Test_Array_PushData.cxx
    Test_Array_AccessOperator.cxx

    # json object
    Test_Ctors_Object.cxx
    Test_Object_AccessOperator.cxx
    Test_Object_InsertData.cxx
    Test_Object_Contains.cxx

    # generic lookup
    Test_Size.cxx
    Test_Empty.cxx

    # generic modifiers
    Test_Clear.cxx

    # decoding
    Test_Decode_Empty.cxx
    Test_Decode_SingleValue.cxx
    Test_Decode_StringEscape.cxx
    Test_Decode_StringInvalidEscape.cxx
    Test_Decode_ArrayUnclosed.cxx
    Test_Decode_ArrayTrailingComma.cxx
    Test_Decode_ObjectUnclosed.cxx
    Test_Decode_ObjectInvalidKey.cxx
    Test_Decode_ObjectTrailingComma.cxx
    Test_Decode_SampleData.cxx
    Test_Decode_InvalidLiteral.cxx

    # encoding
    TestEncode.cxx
)

create_test_sourcelist(
    TEST_SOURCELIST
    CommonCxxTests.cxx
    ${SOURCE_FILES}
)

#
# Target
#

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    CommonCxxTests
    ${TEST_SOURCELIST}
)

set_target_properties(
    CommonCxxTests
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_compile_options(
    CommonCxxTests
    PRIVATE -Wextra -Wall -pedantic
)

target_include_directories(
    CommonCxxTests
    PRIVATE $<TARGET_FILE_DIR:${PROJECT_NAME}>/include
)

target_link_libraries(
    CommonCxxTests
    PRIVATE ${PROJECT_NAME}
)

foreach(TEST_FILE ${SOURCE_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_test(
        NAME ${TEST_NAME}
        COMMAND CommonCxxTests ${TEST_NAME}
    )
endforeach()

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/PostBuild.cmake)
