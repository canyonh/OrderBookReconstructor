find_package(benchmark)

set(BENCHMARK_SRC_FILES
)

set(BENCHMARK_HARNESS_FILES
    ${ROOT_DIR}/test/benchmark/benchmark.cpp
    ${ROOT_DIR}/test/benchmark/queue_benchmark.cpp
)

add_executable(benchmark
    ${BENCHMARK_SRC_FILES}
    ${BENCHMARK_HARNESS_FILES}
)

target_link_libraries(benchmark PRIVATE benchmark::benchmark)

target_include_directories(benchmark PRIVATE
    ${ROOT_DIR}/src
)

set_target_properties(benchmark PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

#add_dependencies(benchmark copy_data_files)
