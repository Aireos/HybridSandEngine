#include "brush.hpp"
#include <cassert>
#include <cmath>
#include <vector>

int main() {
    constexpr uint32_t capacity = 128;
    std::vector<Particle> particles(capacity);
    uint32_t count = 0;

    Brush::paint_circle(particles, count, capacity, glm::vec2(10.0f, 10.0f), 2.0f, Material::SAND);
    assert(count > 0);
    for (uint32_t i = 0; i < count; ++i) {
        assert(particles[i].material == Material::SAND);
        assert(glm::length(particles[i].position - glm::vec2(10.0f, 10.0f)) <= 2.01f);
    }

    particles[count++] = {glm::vec2(20.0f, 20.0f), glm::vec2(0.0f), Material::STONE, 255, 0, 0};
    const uint32_t painted_count = count - 1;
    Brush::erase_circle(particles, count, glm::vec2(10.0f, 10.0f), 2.0f);
    assert(count == 1);
    assert(particles[0].material == Material::STONE);
    assert(std::abs(particles[0].position.x - 20.0f) < 0.01f);
    assert(count < painted_count + 1);
    return 0;
}