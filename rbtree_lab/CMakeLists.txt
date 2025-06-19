cmake_minimum_required(VERSION 3.10)
project(rbtree_lab C)

# 실제 프로그램 실행 (main만 있는 driver.c)
add_executable(rbtree_main
        src/driver.c
        src/rbtree.c
)

# 테스트 실행 파일 (테스트용 main 포함)
add_executable(test_rbtree
        test/test-rbtree.c
        src/rbtree.c
)
