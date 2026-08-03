add_custom_command(
    TARGET CommonCxxTests
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/TestFiles
            $<TARGET_FILE_DIR:CommonCxxTests>/TestFiles
        COMMENT "Copying test files to build directory..."
)
