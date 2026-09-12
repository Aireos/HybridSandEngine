
# Migration Guide: HybridSandEngine Repository Split

## Overview

The HybridSandEngine repository has been split into two independent repositories following a clean modular architecture:

1. **HybridSandCore** - Reusable engine library (simulation, physics, rendering)
2. **HybridSandEngine** - Interactive sandbox application (UI, input, demo)

This guide explains the changes and how to work with the new structure.

---

## What Changed

### Before (Monolithic)
```
HybridSandEngine/
├── src/
│   ├── main.cpp (entry point)
│   ├── engine.cpp (window/input)
│   ├── world.cpp (CORE)
│   ├── particle_system.cpp (CORE)
│   ├── physics.cpp (CORE)
│   ├── gpu_compute.cpp (CORE)
│   ├── renderer.cpp (CORE)
│   ├── brush.cpp (CORE)
│   └── thread_pool.cpp (CORE)
├── include/
│   └── (all headers mixed)
└── CMakeLists.txt (single target)
```

### After (Modular)
```
HybridSandCore/ (NEW REPOSITORY)
├── src/ (all core simulation files)
├── include/ (all core headers)
└── CMakeLists.txt (builds static library)

HybridSandEngine/ (SANDBOX - THIS REPOSITORY)
├── src/
│   ├── main.cpp (entry point)
│   └── engine.cpp (window/input only)
├── include/
│   └── engine.hpp
└── CMakeLists.txt (depends on HybridSandCore)
```

---

## New Dependencies

### For HybridSandEngine (Sandbox)

The sandbox now depends on **HybridSandCore** library. When building:

```bash
# Step 1: Build and install HybridSandCore
git clone https://github.com/Aireos/HybridSandCore.git
cd HybridSandCore
cmake -S . -B build
cmake --build build
cmake --install build  # Install to system or local prefix

# Step 2: Build HybridSandEngine
cd ../HybridSandEngine
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/install
cmake --build build
./build/HybridSandEngine
```

### Headers

**Before:**
```cpp
#include "world.hpp"
#include "particle_system.hpp"
#include "physics.hpp"
```

**After (in sandbox):**
```cpp
#include <HybridSandCore/world.hpp>
#include <HybridSandCore/particle_system.hpp>
#include <HybridSandCore/physics.hpp>
```

**If using HybridSandCore as a library:**
```cpp
#include "world.hpp"  // Include paths configured by CMake
#include "particle_system.hpp"
#include "physics.hpp"
```

---

## Migration Steps for Existing Projects

### If you were using HybridSandEngine

**Option 1: Use as Sandbox (Recommended for demos)**
```bash
git clone https://github.com/Aireos/HybridSandCore.git
git clone https://github.com/Aireos/HybridSandEngine.git

cd HybridSandCore && cmake -S . -B build && cmake --build build
cmake --install build
cd ../HybridSandEngine && cmake -S . -B build && cmake --build build
```

**Option 2: Use HybridSandCore as library (Recommended for tools/editors)**
```cmake
# In your CMakeLists.txt
find_package(HybridSandCore REQUIRED)

add_executable(MyApplication ...)
target_link_libraries(MyApplication PRIVATE HybridSandCore)
```

Then in your code:
```cpp
#include <HybridSandCore/world.hpp>
#include <HybridSandCore/common.hpp>

int main() {
    World world(1024, 1024);
    world.spawn_particle(glm::vec2(512, 512), Material::SAND);
    world.tick(0.016f);
    world.render();
    return 0;
}
```

---

## File Locations Reference

### Moved to HybridSandCore

| File | Location | Status |
|------|----------|--------|
| world.hpp | HybridSandCore/include/ | ✓ Moved |
| world.cpp | HybridSandCore/src/ | ✓ Moved |
| particle_system.hpp | HybridSandCore/include/ | ✓ Moved |
| particle_system.cpp | HybridSandCore/src/ | ✓ Moved |
| gpu_compute.hpp | HybridSandCore/include/ | ✓ Moved |
| gpu_compute.cpp | HybridSandCore/src/ | ✓ Moved |
| physics.hpp | HybridSandCore/include/ | ✓ Moved |
| physics.cpp | HybridSandCore/src/ | ✓ Moved |
| renderer.hpp | HybridSandCore/include/ | ✓ Moved |
| renderer.cpp | HybridSandCore/src/ | ✓ Moved |
| thread_pool.hpp | HybridSandCore/include/ | ✓ Moved |
| thread_pool.cpp | HybridSandCore/src/ | ✓ Moved |
| brush.hpp | HybridSandCore/include/ | ✓ Moved |
| brush.cpp | HybridSandCore/src/ | ✓ Moved |
| common.hpp | HybridSandCore/include/ | ✓ Moved |

### Stayed in HybridSandEngine

| File | Location | Status |
|------|----------|--------|
| engine.hpp | HybridSandEngine/include/ | ✓ Here |
| engine.cpp | HybridSandEngine/src/ | ✓ Here |
| main.cpp | HybridSandEngine/src/ | ✓ Here |

---

## CMake Changes

### Before
```cmake
cmake_minimum_required(VERSION 3.16)
project(HybridSandEngine LANGUAGES CXX)

# ... dependencies ...

# Single library containing everything
add_library(HybridSandCore STATIC ${CORE_SOURCES})
add_executable(HybridSandEngine src/main.cpp src/engine.cpp)
target_link_libraries(HybridSandEngine PRIVATE HybridSandCore)
```

### After (HybridSandCore)
```cmake
cmake_minimum_required(VERSION 3.16)
project(HybridSandCore LANGUAGES CXX)

# ... dependencies ...

# Standalone reusable library
add_library(HybridSandCore STATIC ${CORE_SOURCES})
install(TARGETS HybridSandCore ...)
```

### After (HybridSandEngine)
```cmake
cmake_minimum_required(VERSION 3.16)
project(HybridSandEngine LANGUAGES CXX)

# ... dependencies ...

# Find the installed HybridSandCore
find_package(HybridSandCore REQUIRED)

# Just the sandbox executable
add_executable(HybridSandEngine src/main.cpp src/engine.cpp)
target_link_libraries(HybridSandEngine PRIVATE HybridSandCore)
```

---

## Benefits of This Split

### For HybridSandCore Users
- ✅ Use engine in multiple projects (editors, different sandboxes, tools)
- ✅ Stable, versioned library API
- ✅ Can install to system/package manager
- ✅ Engine development independent of UI/demos

### For HybridSandEngine Users
- ✅ Cleaner, minimal sandbox code
- ✅ Easy to extend (add new tools, UI, features)
- ✅ Demonstrates engine usage patterns
- ✅ Can switch to different core versions easily

---

## Troubleshooting

### CMake can't find HybridSandCore

**Solution:** Specify the install path:
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/hybridcore/install
cmake --build build
```

Or install to standard location:
```bash
cd HybridSandCore
cmake --install build --prefix /usr/local
```

### Header include errors

**Error:** `#include "world.hpp"` not found

**Solution:** Use full include path in sandbox:
```cpp
#include <HybridSandCore/world.hpp>
```

Or in your own project using the library:
```cpp
#include "world.hpp"  // CMake will configure include paths
```

### Linker errors

**Error:** `undefined reference to 'World::World'`

**Solution:** Ensure HybridSandCore is linked:
```cmake
find_package(HybridSandCore REQUIRED)
target_link_libraries(YourProject PRIVATE HybridSandCore)
```

---

## Development Workflow

### Working on Engine Features

1. Clone and develop in **HybridSandCore**
2. Build tests: `cmake --build build && ctest --test-dir build`
3. Install locally: `cmake --install build --prefix ./install`
4. Test in sandbox: `cd ../HybridSandEngine && cmake -S . -B build -DCMAKE_PREFIX_PATH=../HybridSandCore/install`

### Working on Sandbox Features

1. Clone **HybridSandCore** and **HybridSandEngine**
2. Install stable HybridSandCore
3. Develop sandbox features in **HybridSandEngine**
4. Use `git submodule` or package manager for HybridSandCore dependency

### Creating a New Application

```bash
# Create project
mkdir MyApplication && cd MyApplication

# Link to HybridSandCore
find_package(HybridSandCore REQUIRED)

# Build against it
cmake -S . -B build
cmake --build build
```

---

## Questions?

- **Engine issues?** → [HybridSandCore issues](https://github.com/Aireos/HybridSandCore/issues)
- **Sandbox issues?** → [HybridSandEngine issues](https://github.com/Aireos/HybridSandEngine/issues)

