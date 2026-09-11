#pragma once

#include "common.hpp"
#include <GL/glew.h>
#include <vector>

class GPUCompute {
public:
    GPUCompute();
    ~GPUCompute();

    // Initialize GPU buffers
    void init(uint32_t num_particles);

    // Upload particle data to GPU
    void upload_particles(const Particle* particles, uint32_t count);

    // Download particle data from GPU
    void download_particles(Particle* particles, uint32_t count);

    // Run falling sand simulation on GPU
    void simulate_particles(float dt, uint32_t num_particles);

    // Update grid state on GPU
    void update_grid(uint32_t width, uint32_t height);

    // Get GPU buffers
    GLuint get_particle_buffer() const { return ssbo_particles_; }
    GLuint get_grid_buffer() const { return ssbo_grid_; }

private:
    GLuint program_simulate_;
    GLuint program_grid_update_;
    GLuint ssbo_particles_;
    GLuint ssbo_grid_;
    GLuint ssbo_velocities_;
    uint32_t max_particles_;

    GLuint compile_shader(const char* compute_source);
};
