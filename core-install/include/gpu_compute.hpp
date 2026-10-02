#pragma once

#include "common.hpp"
#include <GL/glew.h>
#include <vector>

class GPUCompute {
public:
    GPUCompute();
    ~GPUCompute();

    // Initialize GPU buffers
    void init(uint32_t initial_capacity, uint32_t grid_width, uint32_t grid_height);

    // Upload particle data to GPU
    void upload_particles(const Particle* particles, uint32_t count, uint32_t first_particle = 0);

    // Download particle data from GPU
    void download_particles(Particle* particles, uint32_t count);

    // Run falling sand simulation on GPU
    void simulate_particles(uint32_t num_particles);

    // Append candidate particles only to currently unoccupied cells.
    uint32_t spawn_particles(const Particle* candidates, uint32_t candidate_count, uint32_t first_particle);

    // Remove particles inside a circle and compact the remaining GPU buffer.
    uint32_t erase_particles(const glm::vec2& center, float radius, uint32_t num_particles);

    // Get GPU buffers
    GLuint get_particle_buffer() const { return ssbo_particles_; }
    GLuint get_grid_buffer() const { return ssbo_grid_; }

private:
    GLuint program_build_grid_;
    GLuint program_simulate_;
    GLuint program_erase_;
    GLuint program_spawn_;
    GLuint ssbo_particles_;
    GLuint ssbo_particles_scratch_;
    GLuint ssbo_grid_;
    GLuint ssbo_particle_count_;
    uint32_t max_particles_;
    uint32_t grid_width_;
    uint32_t grid_height_;
    GLint build_grid_num_particles_location_;
    GLint build_grid_width_location_;
    GLint build_grid_height_location_;
    GLint simulate_num_particles_location_;
    GLint simulate_grid_width_location_;
    GLint simulate_grid_height_location_;
    GLint simulate_step_location_;
    GLint erase_num_particles_location_;
    GLint erase_center_location_;
    GLint erase_radius_squared_location_;
    GLint erase_grid_width_location_;
    GLint erase_grid_height_location_;
    GLint spawn_candidate_count_location_;
    GLint spawn_first_particle_location_;
    GLint spawn_grid_width_location_;
    GLint spawn_grid_height_location_;

    void ensure_particle_capacity(uint32_t required_capacity);
    GLuint compile_shader(const char* compute_source);
};
