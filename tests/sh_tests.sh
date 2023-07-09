


# reset
# make -j6

# # ./bin/exec --gtest_filter=basic_double_linked_list__swap_two_links_from_same_list*
# # ./bin/exec --gtest_filter=physic_wrapper*
# # ./bin/exec

# # ./bin/exec --gtest_filter=*generic_array_container*

# # ./bin/exec --gtest_filter=basic_regexp_parser*
# valgrind --leak-check=full ./bin/exec --gtest_filter=basic_regexp_parser*

# # ./bin/exec


# exit 0


reset

CURR_DIR=$PWD
cd ../
# make build_platform="native" all -j3
# make build_mode="debug" build_platform="native" all -j3
make build_mode="release" build_platform="native" all -j3
cd $CURR_DIR

# make build_mode="debug" build_platform="native" framework fclean
# make build_mode="debug" build_platform="native" all -j3
make build_mode="release" build_platform="native" all -j3

# make build_mode="debug" build_platform="native" fclean_tests
# make build_mode="debug" build_platform="native" build-tests -j6

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
./bin/exec


