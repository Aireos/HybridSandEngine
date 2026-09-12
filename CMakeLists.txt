cmake_minimum_required(VERSION 3.16)

project(HybridSandEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Find HybridSandCore library
find_package(HybridSandCore REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sandbox executable - depends on HybridSandCore
add_executable(HybridSandEngine 
    src/main.cpp 
    src/engine.cpp 
    include/engine.hpp
)

target_link_libraries(HybridSandEngine PRIVATE 
    HybridSandCore
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glm::glm
    Threads::Threads
)

# Compiler optimizations
if(MSVC)
    target_compile_options(HybridSandEngine PRIVATE /O2 /arch:AVX2)
else()
    target_compile_options(HybridSandEngine PRIVATE -O3 -march=native -ffast-math)
endif()
