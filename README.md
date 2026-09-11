# Hybrid Sand Engine

A high-performance falling sand simulation engine targeting 60+ FPS with 10M+ particles.

## Architecture

### CPU/GPU Hybrid System
- **CPU-based physics** (CTPL thread pool) for precise collision detection and material interactions
- **GPU compute shaders** for parallel particle updates at massive scales
- **Adaptive switching**: CPU for <100K particles, GPU for >100K particles
- **Grid-based spatial acceleration** for efficient collision queries

### Components

1. **ParticleSystem** - Manages particle state and dispatching
2. **GPUCompute** - Compute shader pipeline for particle simulation
3. **ThreadPool** - CTPL-based thread pool for CPU physics
4. **World** - Game world state and entity management
5. **Renderer** - OpenGL 4.3+ rendering pipeline
6. **Physics** - Material properties and collision response

## Performance Targets

- **10M+ particles** at 60 FPS
- **Grid resolution** 1024x1024
- **Multi-threaded CPU** physics with 8-thread pool
- **GPU compute** shader pipeline (4.3+)

## Building

### Dependencies
- CMake 3.16+
- SDL2
- GLEW
- OpenGL 4.3+
- GLM
- C++17 compiler

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j8
./HybridSandEngine
```

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
msvc HybridSandEngine.sln
# Build in Visual Studio
```

## Usage

The engine spawns 1M sand particles on startup and simulates them falling under gravity.

- **ESC** - Quit
- Console output shows FPS, particle count, and CPU timing

## Materials

- **Sand** (1600 kg/m³, 0.3 friction)
- **Water** (1000 kg/m³, 0.5 friction)
- **Stone** (2500 kg/m³, 0.2 friction)
- **Wood** (600 kg/m³, 0.4 friction)
- **Fire** (reactive)
- **Acid** (reactive)

## Optimization Notes

### CPU Optimizations
- 32-byte particle struct (cache-aligned)
- Thread pool with lock-free queuing
- Spatial grid for collision queries
- SIMD-friendly data layout

### GPU Optimizations
- Compute shader local size 256 (optimal for modern GPUs)
- Persistent buffer mapping for zero-copy uploads
- Workgroup memory for particle interactions
- Memory barriers minimized

## Future Improvements

- [ ] Material reactions (water/fire interaction)
- [ ] Particle pooling and recycling
- [ ] Adaptive LOD based on density
- [ ] Persistent boundary conditions
- [ ] Pressure-based flow simulation
- [ ] Destruction/building tools
- [ ] Save/load world state
- [ ] Network multiplayer support

## License

MIT
