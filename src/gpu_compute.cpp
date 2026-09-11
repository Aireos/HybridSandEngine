#include "gpu_compute.hpp"
#include <iostream>
#include <string>

const char* COMPUTE_SHADER_SIMULATE = R"(
#version 430 core
layout(local_size_x = 256) in;

struct Particle {
    vec2 position;
    vec2 velocity;
    uint material;
    uint life;
    uint temperature;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform float dt;
uniform uint num_particles;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= num_particles) return;
    
    Particle p = particles[idx];
    
    // Apply gravity
    p.velocity.y += 9.81 * dt;
    
    // Apply friction
    p.velocity *= 0.99;
    
    // Update position
    p.position += p.velocity * dt;
    
    // Bounce off boundaries
    if (p.position.y > 1024.0) {
        p.position.y = 1024.0;
        p.velocity.y = -p.velocity.y * 0.8;
    }
    if (p.position.x < 0.0) {
        p.position.x = 0.0;
        p.velocity.x = -p.velocity.x * 0.8;
    }
    if (p.position.x > 1024.0) {
        p.position.x = 1024.0;
        p.velocity.x = -p.velocity.x * 0.8;
    }
    
    particles[idx] = p;
}
)";

GPUCompute::GPUCompute() 
    : program_simulate_(0), program_grid_update_(0), 
      ssbo_particles_(0), ssbo_grid_(0), ssbo_velocities_(0),
      max_particles_(0) {}

GPUCompute::~GPUCompute() {
    if (ssbo_particles_) glDeleteBuffers(1, &ssbo_particles_);
    if (ssbo_grid_) glDeleteBuffers(1, &ssbo_grid_);
    if (ssbo_velocities_) glDeleteBuffers(1, &ssbo_velocities_);
    if (program_simulate_) glDeleteProgram(program_simulate_);
    if (program_grid_update_) glDeleteProgram(program_grid_update_);
}

void GPUCompute::init(uint32_t num_particles) {
    max_particles_ = num_particles;
    
    // Compile compute shader
    program_simulate_ = compile_shader(COMPUTE_SHADER_SIMULATE);
    
    // Create SSBOs
    glGenBuffers(1, &ssbo_particles_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_particles_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 
                 num_particles * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_particles_);
    
    glGenBuffers(1, &ssbo_grid_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_grid_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 
                 GRID_WIDTH * GRID_HEIGHT * sizeof(GridCell), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_grid_);
}

void GPUCompute::upload_particles(const Particle* particles, uint32_t count) {
    glBindBuffer(GL_COPY_WRITE_BUFFER, ssbo_particles_);
    glBufferSubData(GL_COPY_WRITE_BUFFER, 0, count * sizeof(Particle), particles);
}

void GPUCompute::download_particles(Particle* particles, uint32_t count) {
    glBindBuffer(GL_COPY_READ_BUFFER, ssbo_particles_);
    glGetBufferSubData(GL_COPY_READ_BUFFER, 0, count * sizeof(Particle), particles);
}

void GPUCompute::simulate_particles(float dt, uint32_t num_particles) {
    glUseProgram(program_simulate_);
    glUniform1f(glGetUniformLocation(program_simulate_, "dt"), dt);
    glUniform1ui(glGetUniformLocation(program_simulate_, "num_particles"), num_particles);
    
    uint32_t groups = (num_particles + 255) / 256;
    glDispatchCompute(groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GPUCompute::update_grid(uint32_t width, uint32_t height) {
    // Grid update happens in particle simulation
}

GLuint GPUCompute::compile_shader(const char* compute_source) {
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &compute_source, nullptr);
    glCompileShader(shader);
    
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Compute shader compilation failed: " << info_log << std::endl;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "Shader program linking failed: " << info_log << std::endl;
    }
    
    glDeleteShader(shader);
    return program;
}
