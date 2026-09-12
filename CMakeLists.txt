cmake_minimum_required(VERSION 3.16)

project(HybridSandEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add thread support
find_package(Threads REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/engine.cpp
    src/world.cpp
    src/particle_system.cpp
    src/gpu_compute.cpp
    src/thread_pool.cpp
    src/renderer.cpp
    src/physics.cpp
)

set(HEADERS
    include/engine.hpp
    include/world.hpp
    include/particle_system.hpp
    include/gpu_compute.hpp
    include/thread_pool.hpp
    include/renderer.hpp
    include/physics.hpp
    include/common.hpp
)

# Create executable
add_executable(HybridSandEngine ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(HybridSandEngine 
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
