


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

# make build_mode="debug" build_platform="native" framework fclean
make build_mode="debug" build_platform="native" framework -j6

# make build_mode="debug" build_platform="native" fclean_tests
# make build_mode="debug" build_platform="native" build-tests -j6

# ./bin/exec
valgrind --leak-check=full ./bin/exec
