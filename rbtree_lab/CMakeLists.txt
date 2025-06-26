#cmake_minimum_required(VERSION 3.10)
#project(rbtree_lab C)
#
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -DSENTINEL")
#
#add_executable(rbtree_main
#        src/driver.c
#        src/rbtree.c
#)
#add_executable(test_rbtree
#        test/test-rbtree.c
#        src/rbtree.c
#)
#target_include_directories(test_rbtree PRIVATE ${CMAKE_SOURCE_DIR}/src)
#target_include_directories(rbtree_main PRIVATE ${CMAKE_SOURCE_DIR}/src)
#
#enable_testing()
#add_test(NAME test_rbtree COMMAND test_rbtree)


cmake_minimum_required(VERSION 3.10)
project(rbtree_lab C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -DSENTINEL")

# rbtree_main 실행파일
add_executable(rbtree_main
        src/driver.c
        src/rbtree.c
)

# test_rbtree 실행파일
add_executable(test_rbtree
        test/test-rbtree.c
        src/rbtree.c
)
target_include_directories(test_rbtree PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(rbtree_main PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 🔽 visualizer 실행파일 추가!
add_executable(visualize_rbtree
        rb-tree-visualizer/visualizer/test/visualize-main.c
        rb-tree-visualizer/visualizer/test/rbtree_visualizer.c
        src/rbtree.c
)
target_include_directories(visualize_rbtree PRIVATE
        rb-tree-visualizer/visualizer/test
        ${CMAKE_SOURCE_DIR}/src
)

# 테스트 설정 (test_rbtree)
enable_testing()
add_test(NAME test_rbtree COMMAND test_rbtree)

