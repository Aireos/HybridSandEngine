#include "world.hpp"
#include <cmath>

World::World(uint32_t width, uint32_t height)
    : width_(width), height_(height) {
    renderer_ = std::make_unique<Renderer>(width, height);
    renderer_->init();
    particle_system_ = std::make_unique<ParticleSystem>(width, height);
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

void World::paint_circle(const glm::vec2& pos, float radius, Material mat) {
    particle_system_->add_particles_in_circle(pos, radius, mat);
}

void World::destroy_circle(const glm::vec2& pos, float radius) {
    particle_system_->remove_particles_in_circle(pos, radius);
}

const PerfStats& World::get_stats() const {
    return particle_system_->get_stats();
}
