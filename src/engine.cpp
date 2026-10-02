#include "engine.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>

Engine::Engine() : running_(false), delta_time_(0.0f), brush_radius_(8.0f) {}

Engine::~Engine() {
    shutdown();
}

void Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }
    
    world_ = std::make_unique<World>(GRID_WIDTH, GRID_HEIGHT);
    
    // Initialize renderer
    running_ = true;
    last_frame_time_ = std::chrono::high_resolution_clock::now();
    
    std::cout << "Engine initialized. Grid: " << GRID_WIDTH << "x" << GRID_HEIGHT << std::endl;
}

int Engine::run() {
    constexpr uint32_t initial_particle_count = 180000;
    constexpr uint32_t initial_row_width = 600;
    for (uint32_t i = 0; i < initial_particle_count; ++i) {
        glm::vec2 pos(
            212.0f + static_cast<float>(i % initial_row_width),
            700.0f + static_cast<float>(i / initial_row_width)
        );
        glm::vec2 vel(0.0f);
        world_->spawn_particle(pos, Material::SAND, vel);
    }
    
    std::cout << "Spawned " << initial_particle_count << " sand particles. Starting simulation..." << std::endl;
    
    uint32_t frame_count = 0;
    while (running_) {
        handle_input();
        update();
        render();
        frame_count++;
        
        if (frame_count % 60 == 0) {
            const auto& stats = world_->get_stats();
            std::cout << "Frame " << frame_count << " | FPS: " << stats.fps 
                      << " | Particles: " << stats.active_particles 
                      << " | CPU: " << stats.cpu_time_ms << "ms" << std::endl;
        }
    }
    
    return 0;
}

void Engine::shutdown() {
    world_.reset();
    SDL_Quit();
    running_ = false;
}

void Engine::handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running_ = false;
                }
                break;
        }
    }

    int mouse_x = 0;
    int mouse_y = 0;
    const Uint32 mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    const glm::vec2 brush_position(
        static_cast<float>(mouse_x),
        static_cast<float>(GRID_HEIGHT - 1 - mouse_y)
    );

    const auto now = std::chrono::high_resolution_clock::now();
    if (now - last_brush_action_time_ < std::chrono::milliseconds(16)) return;

    if ((mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
        world_->paint_circle(brush_position, brush_radius_, Material::SAND);
        last_brush_action_time_ = now;
    } else if ((mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0) {
        world_->destroy_circle(brush_position, brush_radius_);
        last_brush_action_time_ = now;
    }
}

void Engine::update() {
    auto now = std::chrono::high_resolution_clock::now();
    delta_time_ = std::chrono::duration<float>(now - last_frame_time_).count();
    last_frame_time_ = now;
    
    world_->tick(delta_time_);
}

void Engine::render() {
    world_->render();
}
