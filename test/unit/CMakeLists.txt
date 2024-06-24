find_package(GTest)

set(unit_test_files
    ${ROOT_DIR}/test/unit/unit_test.cpp
)

add_executable(unit_test
    ${unit_test_files}
)

target_include_directories(unit_test PRIVATE
    ${ROOT_DIR}/src
)

target_link_libraries(unit_test PRIVATE GTest::GTest)

set_target_properties(unit_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)


