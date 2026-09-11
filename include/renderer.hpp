#pragma once

#include "common.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>

class Renderer {
public:
    Renderer(uint32_t width, uint32_t height);
    ~Renderer();

    // Initialize rendering
    void init();

    // Render particles
    void render_particles(const Particle* particles, uint32_t count);

    // Render grid (debug)
    void render_grid(const GridCell* grid, uint32_t width, uint32_t height);

    // Render UI and stats
    void render_ui(const PerfStats& stats);

    // Swap buffers
    void present();

    // Clear screen
    void clear();

    SDL_Window* get_window() const { return window_; }

private:
    SDL_Window* window_;
    SDL_GLContext gl_context_;
    uint32_t width_;
    uint32_t height_;

    GLuint vao_particles_;
    GLuint vbo_positions_;
    GLuint vbo_materials_;
    GLuint program_render_;

    GLuint compile_shader(GLenum type, const char* source);
    GLuint create_program(const char* vert_src, const char* frag_src);
};
