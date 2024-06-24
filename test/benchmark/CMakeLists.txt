find_package(benchmark)

set(BENCHMARK_FRAMEWORK_FILES
    ${ROOT_DIR}/test/benchmark/benchmark.cpp
)

set(BENCHMARK_BENCHMARK_FILES
)

add_executable(benchmark
    ${BENCHMARK_FRAMEWORK_FILES}
    ${BENCHMARK_BENCHMARK_FILES}
)

target_link_libraries(benchmark PRIVATE benchmark::benchmark)

target_include_directories(benchmark PRIVATE
    ${ROOT_DIR}/src
)

set_target_properties(benchmark PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

#add_dependencies(benchmark copy_data_files)
