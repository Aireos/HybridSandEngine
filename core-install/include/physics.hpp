#pragma once

#include "common.hpp"
#include <glm/glm.hpp>

class Physics {
public:
    // Apply gravity to particle
    static void apply_gravity(Particle& p, float dt);

    // Check grid collision and resolve
    static bool check_collision(const Particle& p, const GridCell* grid, uint32_t grid_width, uint32_t grid_height);

    // Apply velocity and friction
    static void integrate(Particle& p, const GridCell* grid, uint32_t grid_width, uint32_t grid_height, float dt);

    // Get material properties
    static float get_friction(Material m);
    static float get_density(Material m);
    static glm::vec3 get_color(Material m);
};
