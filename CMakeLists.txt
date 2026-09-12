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

# Core library sources
set(CORE_SOURCES
    src/world.cpp
    src/particle_system.cpp
    src/gpu_compute.cpp
    src/thread_pool.cpp
    src/renderer.cpp
    src/physics.cpp
    src/brush.cpp
)

# Core library headers
set(CORE_HEADERS
    include/world.hpp
    include/particle_system.hpp
    include/gpu_compute.hpp
    include/thread_pool.hpp
    include/renderer.hpp
    include/physics.hpp
    include/common.hpp
    include/brush.hpp
)

# Simulation and engine modules are reusable by the sandbox and future tools.
add_library(HybridSandCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# The executable is the current interactive sandbox: input, window, and app loop.
add_executable(HybridSandEngine src/main.cpp src/engine.cpp include/engine.hpp)

set(COMMON_LIBRARIES
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glm::glm
    Threads::Threads
)

target_link_libraries(HybridSandCore PUBLIC ${COMMON_LIBRARIES})
target_link_libraries(HybridSandEngine PRIVATE HybridSandCore)

enable_testing()
add_executable(BrushTest tests/brush_test.cpp src/brush.cpp)
target_link_libraries(BrushTest PRIVATE glm::glm)
add_test(NAME BrushTest COMMAND BrushTest)

# Compiler optimizations
if(MSVC)
    target_compile_options(HybridSandCore PRIVATE /O2 /arch:AVX2)
    target_compile_options(HybridSandEngine PRIVATE /O2 /arch:AVX2)
else()
    target_compile_options(HybridSandCore PRIVATE -O3 -march=native -ffast-math)
    target_compile_options(HybridSandEngine PRIVATE -O3 -march=native -ffast-math)
endif()
