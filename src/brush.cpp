#include "brush.hpp"
#include <algorithm>
#include <cmath>

namespace Brush {
void paint_circle(
    std::vector<Particle>& particles,
    uint32_t& particle_count,
    uint32_t max_particles,
    const glm::vec2& center,
    float radius,
    Material material
) {
    const float radius_squared = radius * radius;
    const int min_x = static_cast<int>(std::floor(center.x - radius));
    const int max_x = static_cast<int>(std::ceil(center.x + radius));
    const int min_y = static_cast<int>(std::floor(center.y - radius));
    const int max_y = static_cast<int>(std::ceil(center.y + radius));

    for (int y = min_y; y <= max_y && particle_count < max_particles; ++y) {
        for (int x = min_x; x <= max_x && particle_count < max_particles; ++x) {
            const glm::vec2 offset(static_cast<float>(x) - center.x, static_cast<float>(y) - center.y);
            if (glm::dot(offset, offset) <= radius_squared) {
                particles[particle_count++] = {
                    glm::vec2(static_cast<float>(x), static_cast<float>(y)),
                    glm::vec2(0.0f),
                    material,
                    255,
                    0,
                    0
                };
            }
        }
    }
}

void erase_circle(
    std::vector<Particle>& particles,
    uint32_t& particle_count,
    const glm::vec2& center,
    float radius
) {
    const float radius_squared = radius * radius;
    uint32_t write_index = 0;

    for (uint32_t read_index = 0; read_index < particle_count; ++read_index) {
        const glm::vec2 offset = particles[read_index].position - center;
        if (glm::dot(offset, offset) > radius_squared) {
            particles[write_index++] = particles[read_index];
        }
    }

    particle_count = write_index;
}
}