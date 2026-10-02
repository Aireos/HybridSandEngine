#pragma once

#include "common.hpp"
#include "particle_system.hpp"
#include "renderer.hpp"
#include <memory>

class World {
public:
    World(uint32_t width, uint32_t height);
    ~World();

    // Update simulation
    void tick(float dt);

    // Render frame
    void render();

    // Add particle at world position
    void spawn_particle(const glm::vec2& pos, Material mat, const glm::vec2& vel = glm::vec2(0));

    // Add material in a world-space circle
    void paint_circle(const glm::vec2& pos, float radius, Material mat);

    // Destroy material in radius (digging)
    void destroy_circle(const glm::vec2& pos, float radius);

    // Get performance stats
    const PerfStats& get_stats() const;

    // Get dimensions
    uint32_t get_width() const { return width_; }
    uint32_t get_height() const { return height_; }

private:
    uint32_t width_;
    uint32_t height_;
    std::unique_ptr<ParticleSystem> particle_system_;
    std::unique_ptr<Renderer> renderer_;
};
