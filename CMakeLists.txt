cmake_minimum_required(VERSION 3.16)

project(HybridSandEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(HybridSandCore CONFIG REQUIRED)

# Sandbox executable - depends on HybridSandCore
add_executable(HybridSandEngine 
    src/main.cpp 
    src/engine.cpp 
    include/engine.hpp
)

target_include_directories(HybridSandEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HybridSandEngine PRIVATE HybridSandCore::HybridSandCore)

# Compiler optimizations
if(MSVC)
    target_compile_options(HybridSandEngine PRIVATE /O2 /arch:AVX2)
else()
    target_compile_options(HybridSandEngine PRIVATE -O3 -march=native -ffast-math)
endif()
