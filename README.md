# Hybrid Sand Engine - Interactive Sandbox

Interactive sandbox application for the Hybrid Sand Engine. Uses the reusable **HybridSandCore** library for high-performance particle simulation.

## Overview

This is the executable sandbox/demo for testing and interacting with the falling sand engine. It handles:

- **Window management** (SDL2)
- **Input handling** (mouse painting/erasing)
- **Application lifecycle** (initialization, main loop, shutdown)
- **Performance monitoring** (FPS, particle count, timing)

All simulation, physics, rendering, and GPU compute logic is in **HybridSandCore**, making this project a clean demonstration of engine usage.

## Usage

### Building

Requires CMake, a C++17 compiler, and the SDL2, GLEW, and GLM development packages. OpenGL and thread support are provided by the platform/toolchain.

```bash
# Keep the two repositories as siblings and install Core into Engine's local prefix.
git clone https://github.com/Aireos/HybridSandCore.git
git clone https://github.com/Aireos/HybridSandEngine.git
cd HybridSandEngine
cd ../HybridSandCore
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../HybridSandEngine/core-install
cmake --build build --config Release
cmake --install build --config Release

cd ../HybridSandEngine
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=core-install
cmake --build build --config Release
```

### Running

```bash
.\build-msys\HybridSandEngine.exe
```

### Controls

- **Left Mouse Button**: Paint sand
- **Right Mouse Button**: Erase particles
- **Escape**: Quit

## Performance

- **1M particles** spawned automatically on startup
- Real-time FPS monitoring (printed to console every 60 frames)
- Adaptive CPU/GPU switching in the core library
- 60 FPS target at 1024x1024 resolution

## Architecture

This sandbox uses the modular HybridSandCore library:

```
HybridSandEngine (Sandbox)
  └── depends on──▶── HybridSandCore
```

The split allows:
- **Reusability**: Core can be used by other applications (editors, tools, different sandboxes)
- **Separation of concerns**: Engine logic vs. UI/application logic
- **Independent versioning**: Core and sandbox can evolve at different rates
- **Library distribution**: Core can be packaged and distributed via package managers

## Future Extensions

This sandbox can be extended with:

- **Material selection UI**: Choose painting material
- **Simulation parameters UI**: Adjust gravity, friction, etc.
- **Save/Load**: Persist world state
- **Advanced tools**: Gravity wells, wind, constraints
- **Editor mode**: Build structures before simulation
- **Network multiplayer**: Shared world simulation

## License

MIT
