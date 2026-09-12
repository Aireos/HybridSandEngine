#pragma once

#include <HybridSandCore/world.hpp>
#include <HybridSandCore/common.hpp>
#include <memory>
#include <chrono>

class Engine {
public:
    Engine();
    ~Engine();

    // Initialize engine
    void init();

    // Main loop
    int run();

    // Shutdown
    void shutdown();

private:
    std::unique_ptr<World> world_;
    bool running_;
    float delta_time_;
    float brush_radius_;
    std::chrono::high_resolution_clock::time_point last_frame_time_;

    // Input handling
    void handle_input();

    // Update world
    void update();

    // Render frame
    void render();
};
