#pragma once

#include "common.hpp"
#include <vector>

namespace Brush {
void paint_circle(
    std::vector<Particle>& particles,
    uint32_t& particle_count,
    uint32_t max_particles,
    const glm::vec2& center,
    float radius,
    Material material
);

void erase_circle(
    std::vector<Particle>& particles,
    uint32_t& particle_count,
    const glm::vec2& center,
    float radius
);
}