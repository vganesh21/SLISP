#!/bin/sh
# assumes root is defined as CMAKE_SOURCE_DIR
echo "Running Coverage assuming source root = " ${ROOT}
lcov -c -i -b ${ROOT} -d . --no-external -o Coverage.baseline
./unittests
./test_gui
./test_message
lcov -c -b ${ROOT} -d . --no-external  -o Coverage.out
lcov -a Coverage.baseline -a Coverage.out -o Coverage.combined
lcov -r Coverage.combined ${ROOT}/catch.hpp -o Coverage.combined.cleaned
genhtml Coverage.combined.cleaned -o Coverage_Report

gcovr -x -r ${ROOT} -e ${ROOT}/catch.hpp --object-directory=./CMakeFiles/unittests.dir/ --object-directory=./CMakeFiles/test_gui.dir/ --object-directory=./CMakeFiles/test_message.dir/ -o coverage.xml
