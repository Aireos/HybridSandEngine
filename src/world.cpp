#include "world.hpp"
#include <cmath>

World::World(uint32_t width, uint32_t height)
    : width_(width), height_(height) {
    particle_system_ = std::make_unique<ParticleSystem>(width, height);
    renderer_ = std::make_unique<Renderer>(width, height);
}

World::~World() {}

void World::tick(float dt) {
    particle_system_->update(dt);
}

void World::render() {
    renderer_->clear();
    
    const auto& particles = particle_system_->get_particles();
    uint32_t count = particle_system_->get_particle_count();
    renderer_->render_particles(particles.data(), count);
    renderer_->render_ui(get_stats());
    renderer_->present();
}

void World::spawn_particle(const glm::vec2& pos, Material mat, const glm::vec2& vel) {
    particle_system_->add_particle(pos, vel, mat);
}

void World::destroy_circle(const glm::vec2& pos, float radius) {
    // Mark particles in radius for deletion
    const auto& particles = particle_system_->get_particles();
    uint32_t count = particle_system_->get_particle_count();
    
    for (uint32_t i = 0; i < count; ++i) {
        glm::vec2 diff = particles[i].position - pos;
        if (glm::length(diff) < radius) {
            // Remove particle (simplified: set life to 0)
            const_cast<Particle&>(particles[i]).life = 0;
        }
    }
}

const PerfStats& World::get_stats() const {
    return particle_system_->get_stats();
}
