cmake_minimum_required(VERSION 3.20)
project(StockMatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Performance flags — all required for competitive latency numbers
# -O3: full optimization
# -march=native: use all CPU features (AVX2, etc.) of your machine
# -fno-exceptions: no exception table overhead on the hot path
# -fno-rtti: no runtime type info overhead
# -DNDEBUG: disable all assert() calls
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-exceptions -fno-rtti -DNDEBUG")

# Separate debug build with sanitizers for correctness testing
set(CMAKE_CXX_FLAGS_DEBUG "-g -O1 -fsanitize=address,undefined")

add_executable(matching_engine
    machine/src/main.cpp
    machine/src/order_book.cpp
    machine/src/matching_engine.cpp
    machine/src/shard.cpp
)
target_include_directories(matching_engine PRIVATE machine/include)

# Benchmarks (no sanitizers, must be Release)
add_executable(bench_matching machine/bench/bench_matching.cpp machine/src/order_book.cpp machine/src/matching_engine.cpp)
target_include_directories(bench_matching PRIVATE machine/include)

# Tests
enable_testing()
add_executable(run_tests
    machine/tests/test_order_book.cpp
    machine/tests/test_matching.cpp
    machine/src/order_book.cpp
    machine/src/matching_engine.cpp
)
target_include_directories(run_tests PRIVATE machine/include)