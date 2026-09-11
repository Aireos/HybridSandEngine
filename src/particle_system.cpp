#include "particle_system.hpp"
#include "physics.hpp"
#include <algorithm>
#include <chrono>

ParticleSystem::ParticleSystem(uint32_t width, uint32_t height)
    : particle_count_(0), grid_width_(width), grid_height_(height) {
    particles_.resize(MAX_PARTICLES);
    particles_staging_.resize(MAX_PARTICLES);
    grid_.resize(width * height, {Material::EMPTY, 0, 0, 0});
    
    thread_pool_ = std::make_unique<ThreadPool>(THREAD_POOL_SIZE);
    gpu_compute_ = std::make_unique<GPUCompute>();
    gpu_compute_->init(MAX_PARTICLES);
    
    stats_ = {0, 0, 0, 0, 0};
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::add_particle(const glm::vec2& pos, const glm::vec2& vel, Material mat) {
    if (particle_count_ < MAX_PARTICLES) {
        particles_[particle_count_] = {
            pos, vel, mat, 255, 0, 0
        };
        particle_count_++;
    }
}

void ParticleSystem::update(float dt) {
    auto frame_start = std::chrono::high_resolution_clock::now();
    
    // Decide CPU vs GPU based on particle count
    if (particle_count_ < 100000) {
        // CPU simulation for small counts
        cpu_update_particles(dt, 0, particle_count_);
    } else {
        // GPU simulation for large counts
        gpu_compute_->upload_particles(particles_.data(), particle_count_);
        gpu_compute_->simulate_particles(dt, particle_count_);
        gpu_compute_->download_particles(particles_.data(), particle_count_);
    }
    
    // Update grid state
    cpu_grid_update();
    
    // Handle collisions and reactions
    uint32_t chunk_size = particle_count_ / THREAD_POOL_SIZE;
    std::vector<std::future<void>> futures;
    
    for (uint32_t i = 0; i < THREAD_POOL_SIZE; ++i) {
        uint32_t start = i * chunk_size;
        uint32_t end = (i == THREAD_POOL_SIZE - 1) ? particle_count_ : (i + 1) * chunk_size;
        
        futures.push_back(thread_pool_->enqueue(
            [this, start, end](int id) {
                for (uint32_t j = start; j < end; ++j) {
                    handle_collisions(j);
                    handle_material_reactions(j);
                }
            }
        ));
    }
    
    // Wait for all threads
    for (auto& f : futures) f.get();
    
    // Update stats
    auto frame_end = std::chrono::high_resolution_clock::now();
    float frame_time = std::chrono::duration<float, std::milli>(frame_end - frame_start).count();
    stats_.cpu_time_ms = frame_time;
    stats_.active_particles = particle_count_;
    stats_.fps = 1000.0f / frame_time;
}

void ParticleSystem::cpu_update_particles(float dt, uint32_t start, uint32_t end) {
    for (uint32_t i = start; i < end; ++i) {
        Physics::integrate(particles_[i], grid_.data(), grid_width_, grid_height_, dt);
    }
}

void ParticleSystem::cpu_grid_update() {
    // Clear grid
    std::fill(grid_.begin(), grid_.end(), GridCell{Material::EMPTY, 0, 0, 0});
    
    // Update grid based on particles
    for (uint32_t i = 0; i < particle_count_; ++i) {
        uint32_t x = static_cast<uint32_t>(particles_[i].position.x);
        uint32_t y = static_cast<uint32_t>(particles_[i].position.y);
        
        if (x < grid_width_ && y < grid_height_) {
            grid_[y * grid_width_ + x].material = particles_[i].material;
            grid_[y * grid_width_ + x].density++;
        }
    }
}

void ParticleSystem::cpu_apply_gravity(float dt) {
    for (uint32_t i = 0; i < particle_count_; ++i) {
        Physics::apply_gravity(particles_[i], dt);
    }
}

void ParticleSystem::handle_collisions(uint32_t particle_idx) {
    Particle& p = particles_[particle_idx];
    uint32_t x = static_cast<uint32_t>(p.position.x);
    uint32_t y = static_cast<uint32_t>(p.position.y);
    
    if (x >= grid_width_ || y >= grid_height_) {
        p.position.x = glm::clamp(p.position.x, 0.0f, static_cast<float>(grid_width_ - 1));
        p.position.y = glm::clamp(p.position.y, 0.0f, static_cast<float>(grid_height_ - 1));
        p.velocity *= 0.8f;
    }
}

void ParticleSystem::handle_material_reactions(uint32_t particle_idx) {
    // Placeholder for material interactions
    // Can add water/sand friction, fire spread, etc.
}
