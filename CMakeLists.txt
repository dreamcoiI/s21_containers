cmake_minimum_required(VERSION 3.25)
project(CPP2_s21_containers_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(code-samples)
include_directories(src)
include_directories(src/s21_containers)
include_directories(src/s21_containers/iterators)

add_executable(CPP2_s21_containers_2
        src/s21_containers/s21_multiset.h
        src/s21_containers/iterators/s21_list_iterator.cpp
        src/s21_containers/iterators/s21_list_iterator.h
        src/s21_containers/s21_list.cpp
        src/s21_containers/s21_list.h
        src/s21_containers/s21_queue.h
        src/s21_containers/s21_stack.h
        src/s21_containers/s21_vector.h
        src
        src/s21_containers.h src/s21_containers/s21_array.h src/s21_containers/s21_tree.h src/s21_containers/s21_map.h src/s21_containers/s21_set.h src/tests/vector_test.cc src/tests/test_main.cc src/tests/test_array.cc src/tests/map_test.cc)
