#include "physics.hpp"
#include <glm/glm.hpp>

void Physics::apply_gravity(Particle& p, float dt) {
    p.velocity.y += GRAVITY * dt;
}

bool Physics::check_collision(const Particle& p, const GridCell* grid, uint32_t grid_width, uint32_t grid_height) {
    uint32_t x = static_cast<uint32_t>(p.position.x);
    uint32_t y = static_cast<uint32_t>(p.position.y);
    
    if (x >= grid_width || y >= grid_height) return true;
    return grid[y * grid_width + x].material != Material::EMPTY;
}

void Physics::integrate(Particle& p, const GridCell* grid, uint32_t grid_width, uint32_t grid_height, float dt) {
    apply_gravity(p, dt);
    
    // Apply friction
    float friction = get_friction(p.material);
    p.velocity *= (1.0f - friction * dt);
    
    // Update position
    glm::vec2 new_pos = p.position + p.velocity * dt;
    
    // Clamp to grid
    new_pos.x = glm::clamp(new_pos.x, 0.0f, static_cast<float>(grid_width - 1));
    new_pos.y = glm::clamp(new_pos.y, 0.0f, static_cast<float>(grid_height - 1));
    
    // Simple collision response
    if (check_collision({new_pos, p.velocity, p.material, p.life, p.temperature, 0}, grid, grid_width, grid_height)) {
        p.velocity *= 0.8f; // Bounce damping
        p.velocity.y = -p.velocity.y * 0.5f;
    } else {
        p.position = new_pos;
    }
}

float Physics::get_friction(Material m) {
    switch (m) {
        case Material::SAND: return 0.3f;
        case Material::WATER: return 0.5f;
        case Material::STONE: return 0.2f;
        case Material::WOOD: return 0.4f;
        default: return 0.1f;
    }
}

float Physics::get_density(Material m) {
    switch (m) {
        case Material::SAND: return 1600.0f;
        case Material::WATER: return 1000.0f;
        case Material::STONE: return 2500.0f;
        case Material::WOOD: return 600.0f;
        default: return 1.0f;
    }
}

glm::vec3 Physics::get_color(Material m) {
    switch (m) {
        case Material::SAND: return {0.76f, 0.70f, 0.50f};
        case Material::WATER: return {0.0f, 0.5f, 1.0f};
        case Material::STONE: return {0.5f, 0.5f, 0.5f};
        case Material::WOOD: return {0.6f, 0.3f, 0.1f};
        case Material::FIRE: return {1.0f, 0.5f, 0.0f};
        case Material::ACID: return {0.0f, 1.0f, 0.0f};
        default: return {1.0f, 1.0f, 1.0f};
    }
}
