#!/bin/sh

# This script tests the CMake build:
#
# - builds the main CMakeLists.txt
# - runs the tests
# - builds and runs a small test app in a separate build tree so
#   the config-module is tested, too
#
# Options (environment variables):
#
# - The variable BUILD_SHARED_LIBS will be forwarded to the CMake project
#   that builds the library.
#
#

set -ex

rm -rf out/include
rm -rf out/lib
rm -rf out/bin

if [ -n "$BUILD_SHARED_LIBS" ]; then
    bsl=-DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS
else
    bsl=-UBUILD_SHARED_LIBS
fi

cmake -H. -Bout/build-fribidi -DCMAKE_INSTALL_PREFIX=${PWD}/out -DCMAKE_BUILD_TYPE=Debug $bsl
cmake --build out/build-fribidi --target install --config Debug --clean-first
cmake out/build-fribidi -DCMAKE_BUILD_TYPE=Release
cmake --build out/build-fribidi --target install --config Release --clean-first

cmake --build out/build-fribidi --target RUN_TESTS --config Release || cmake --build out/build-fribidi --target test --config Release

cmake -Hcmake/testbuild -Bout/cmake-testbuild -DCMAKE_INSTALL_PREFIX=${PWD}/out -DCMAKE_PREFIX_PATH=${PWD}/out -DCMAKE_BUILD_TYPE=Debug
cmake --build out/cmake-testbuild --target install --config Debug --clean-first

cmake out/cmake-testbuild -DCMAKE_BUILD_TYPE=Release
cmake --build out/cmake-testbuild --target install --config Release --clean-first

export LD_LIBRARY_PATH=${PWD}/out/lib:$LD_LIBRARY_PATH

out/bin/cmake-test_d
out/bin/cmake-test

