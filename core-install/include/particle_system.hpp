#pragma once

#include "common.hpp"
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
    const std::vector<Particle>& get_particles() const;
    uint32_t get_particle_count() const { return particle_count_; }
    GLuint get_particle_buffer() const { return gpu_compute_->get_particle_buffer(); }

    // Performance stats
    const PerfStats& get_stats() const { return stats_; }

private:
    mutable std::vector<Particle> particles_;
    uint32_t particle_count_;
    uint32_t grid_width_;
    uint32_t grid_height_;
    bool gpu_state_valid_;

    std::unique_ptr<GPUCompute> gpu_compute_;

    PerfStats stats_;

    void sync_particles_from_gpu() const;
};
