#cmake_minimum_required(VERSION 3.10)

set(unit_test_files
    ${ROOT_DIR}/test/unit/main.cpp
)

add_executable(unit_test
    ${unit_test_files}
)

target_include_directories(unit_test PRIVATE
    ${ROOT_DIR}/OrderbookReconstructor/test/unit
)
