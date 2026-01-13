#!/bin/bash

clear

echo ""
echo "############################"
echo "# ENSURE DEBUG WRAPPER LIB #"
echo "############################"
echo ""

make -f Makefile build_platform=native build_mode=debug -j6 || exit 1

# make -f Makefile-unit-tests fclean

echo ""
echo "###########################"
echo "# RUN SYSTEM MODULE TESTS #"
echo "###########################"
echo ""

make -f Makefile-unit-tests tests_geronimo_system -j6 || exit 1
./bin/test_system || exit 1

echo ""
echo "#############################"
echo "# RUN GRAPHICS MODULE TESTS #"
echo "#############################"
echo ""

make -f Makefile-unit-tests tests_geronimo_graphics -j6 || exit 1
./bin/test_graphics || exit 1

echo ""
echo "############################"
echo "# RUN PHYSICS MODULE TESTS #"
echo "############################"
echo ""

make -f Makefile-unit-tests tests_geronimo_physics -j6 || exit 1
./bin/test_physics || exit 1

echo ""
echo "##########################"
echo "# RUN AUDIO MODULE TESTS #"
echo "##########################"
echo ""

make -f Makefile-unit-tests tests_geronimo_audio -j6 || exit 1
./bin/test_audio || exit 1

echo ""
echo "########"
echo "# DONE #"
echo "########"
echo ""




# ./bin/exec
# valgrind --leak-check=full ./bin/exec
# valgrind --leak-check=full ./bin/exec --gtest_filter=angles*


# valgrind --leak-check=full ./bin/exec --gtest_filter=system_weak_ref_data_pool*
# valgrind --leak-check=full ./bin/exec --gtest_filter=system_basic_double_linked_list*

# valgrind --leak-check=full ./bin/exec --gtest_filter=test_perspective_clustering*
# ./bin/exec --gtest_filter=test_perspective_clustering*

# ./bin/exec --gtest_filter=audio_general_test*
# ./bin/exec --gtest_filter=physic_wrapper*

# ./bin/exec --gtest_filter=physic_wrapper_perf*
# ./bin/exec
# ./bin/exec --gtest_filter=angles.*
# ./bin/exec --gtest_filter=filtering.*

# valgrind ./bin/exec --gtest_filter=physic_wrapper.*
# valgrind ./bin/exec --gtest_filter=system_multithreading.*
# ./bin/exec --gtest_filter=system_multithreading.*





