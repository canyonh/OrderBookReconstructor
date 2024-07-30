cmake_minimum_required(VERSION 3.10)
project(OrderbookReconstructor)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

add_subdirectory(src)
add_subdirectory(test)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS_COMMON "-Wall -Wextra -Werror -fno-omit-frame-pointer --param hardware_destructive_interference_size=64")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_COMMON} -O0 -ggdb3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_COMMON} -O3 -DNDEBUG -ggdb3")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_COMMON "-Wall -Wextra -Werror -fno-omit-frame-pointer -DDESTRUCTIVE_INTERFERENCE_SIZE=64")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_COMMON} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_COMMON} -O3 -DNDEBUG -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
else()
    message(FATAL_ERROR, "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

add_custom_target(copy_data_files
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/data" "${BIN_DIR}/data"
    COMMENT "Copying data files to ${BIN_DIR}/data"
)


