cmake_minimum_required(VERSION 3.24)

#
# Target executables
#

set(
    SOURCE_FILES

    # json class
    TestCtorsBool.cxx
    TestCtorsInt.cxx
    TestCtorsDouble.cxx
    TestCtorsString.cxx
    TestCtorsArray.cxx
    TestArrayPushData.cxx
    TestArrayAccessOperator.cxx
    TestCtorsObject.cxx
    TestObjectAccessOperator.cxx
    TestObjectInsertData.cxx
    TestSize.cxx

    # json decoding
    TestDecodeEmpty.cxx
    TestDecodeSingleValue.cxx
    TestDecodeStringEscape.cxx
    TestDecodeStringInvalidEscape.cxx
    TestDecodeArrayUnclosed.cxx
    TestDecodeArrayTrailingComma.cxx
    TestDecodeObjectUnclosed.cxx
    TestDecodeObjectInvalidKey.cxx
    TestDecodeObjectTrailingComma.cxx
    TestDecodeSampleData.cxx
    TestDecodeInvalidLiteral.cxx

    # json encoding
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
