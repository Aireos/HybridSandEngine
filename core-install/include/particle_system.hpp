#pragma once

#include "common.hpp"
#include "thread_pool.hpp"
#include "gpu_compute.hpp"
#include <vector>
#include <memory>

class ParticleSystem {
public:
    ParticleSystem(uint32_t width, uint32_t height);
    ~ParticleSystem();

    // Add particle to simulation
    void add_particle(const glm::vec2& pos, const glm::vec2& vel, Material mat);

    // Add particles inside a world-space circle
    void add_particles_in_circle(const glm::vec2& pos, float radius, Material mat);

    // Remove particles inside a world-space circle
    void remove_particles_in_circle(const glm::vec2& pos, float radius);

    // Simulate one frame
    void update(float dt);

    // Get particle data
    const std::vector<Particle>& get_particles() const { return particles_; }
    uint32_t get_particle_count() const { return particle_count_; }

    // Performance stats
    const PerfStats& get_stats() const { return stats_; }

private:
    std::vector<Particle> particles_;
    std::vector<Particle> particles_staging_;
    std::vector<GridCell> grid_;
    uint32_t particle_count_;
    uint32_t grid_width_;
    uint32_t grid_height_;

    std::unique_ptr<ThreadPool> thread_pool_;
    std::unique_ptr<GPUCompute> gpu_compute_;

    PerfStats stats_;

    // CPU-side physics for low particle counts
    void cpu_update_particles(float dt, uint32_t start, uint32_t end);
    void cpu_grid_update();
    void cpu_apply_gravity(float dt);

    // Material interactions
    void handle_collisions(uint32_t particle_idx);
    void handle_material_reactions(uint32_t particle_idx);
};
