#!/bin/bash -x

BUILD_ROOT="build"
mkdir -p ${BUILD_ROOT}
REQUESTED_BUILD_TYPE=${1-"debug"}


case $REQUESTED_BUILD_TYPE in 
    debug)
        CMAKE_BUILD_TYPE="Debug"
        ;;
    release)
        CMAKE_BUILD_TYPE="RelWithDebInfo"
        ;;
    *)
        echo "Invalid build type ${REQUESTED_BUILD_TYPE}"
        exit 1;
        ;;
esac

CMAKE_BUILD_DIR=${CMAKE_BUILD_TYPE}
echo "Build type: ${CMAKE_BUILD_TYPE}"

# conan related
CONAN_PROFILE_DIR="toolchain"

# convert Debug -> debug
CONAN_PROFILE_TYPE="$(echo ${CMAKE_BUILD_TYPE} | tr '[A-Z]' '[a-z]')"

CONAN_PROFILE_PATH="${CONAN_PROFILE_DIR}/linux64_gcc12_${CONAN_PROFILE_TYPE}.profile"
#CONAN_PROFILE_PATH="${CONAN_PROFILE_DIR}/linux64_clang14_${CONAN_PROFILE_TYPE}.profile"
echo "conan profile: ${CONAN_PROFILE_PATH}"

CMAKE_TOOLCHAIN_FILE="${CONAN_PROFILE_DIR}/linux64_gcc12.cmake"
#CMAKE_TOOLCHAIN_FILE="${CONAN_PROFILE_DIR}/linux64_clang14.cmake"
echo "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}"

# switch to build directory and build
pushd .
cd ${BUILD_ROOT}

conan install .. --output-folder=${CMAKE_BUILD_TYPE} --build=missing --profile=../${CONAN_PROFILE_PATH}
(cd ${CMAKE_BUILD_TYPE} && cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=on \
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    && make VERBOSE=1 -j`nproc`)
popd

ln -sf ${BUILD_ROOT}/${CMAKE_BUILD_TYPE}/compile_commands.json
