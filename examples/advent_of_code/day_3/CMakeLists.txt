add_executable(advent_3 main.cpp)
target_link_libraries(advent_3 safe_arithmetic range-v3)

#target_compile_options(advent_3 PUBLIC -O3 -fsanitize=address,undefined,cfi -flto -fvisibility=default)
#target_link_options(advent_3 PUBLIC -fsanitize=address,undefined,cfi -flto)