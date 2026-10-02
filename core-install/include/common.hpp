#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

const uint32_t GRID_WIDTH = 1024;
const uint32_t GRID_HEIGHT = 1024;
const uint32_t MAX_PARTICLES = 10000000; // 10M particles
const uint32_t PARTICLE_CHUNK_SIZE = 65536; // 64K per chunk
const float GRAVITY = 9.81f;
const float TIME_STEP = 0.016f; // ~60 FPS
const uint32_t THREAD_POOL_SIZE = 8;

// Material types
enum class Material : uint32_t {
    EMPTY = 0,
    SAND = 1,
    WATER = 2,
    STONE = 3,
    WOOD = 4,
    FIRE = 5,
    ACID = 6,
    COUNT = 7
};

// Particle state
struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    Material material;
    uint32_t life;
    uint32_t temperature;
    uint32_t padding;
};

static_assert(sizeof(Particle) == 32, "Particle struct must be 32 bytes for cache alignment");

// Grid cell
struct GridCell {
    Material material;
    uint8_t density;
    uint8_t temperature;
    uint8_t flags;
};

// Performance counters
struct PerfStats {
    float fps;
    float cpu_time_ms;
    float gpu_time_ms;
    uint32_t active_particles;
    uint32_t updated_cells;
};
