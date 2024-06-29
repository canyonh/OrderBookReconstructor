find_package(GTest)

set(UNITTEST_SRC_FILES
    ${ROOT_DIR}/src/Orderbook/orderbook.cpp
)

set(UNITTEST_HARNESS_FILES
    ${ROOT_DIR}/test/unit/unit_test.cpp
    ${ROOT_DIR}/test/unit/thread_safe_queue_test.cpp
    ${ROOT_DIR}/test/unit/orderbook_test.cpp
)

add_executable(unit_test
    ${UNITTEST_SRC_FILES}
    ${UNITTEST_HARNESS_FILES}
)

target_include_directories(unit_test PRIVATE
    ${ROOT_DIR}/src
)

target_link_libraries(unit_test PRIVATE GTest::GTest)

set_target_properties(unit_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)


