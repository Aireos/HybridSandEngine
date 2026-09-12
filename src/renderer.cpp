#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <stdexcept>

const char* VERTEX_SHADER = R"(
#version 430 core
layout(location = 0) in vec2 position;
layout(location = 1) in uint material;

out VS_OUT {
    uint material;
} vs_out;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
    vs_out.material = material;
}
)";

const char* FRAGMENT_SHADER = R"(
#version 430 core
in VS_OUT {
    uint material;
} fs_in;

out vec4 color;

vec3 get_material_color(uint material) {
    switch(material) {
        case 1u: return vec3(0.76, 0.70, 0.50); // Sand
        case 2u: return vec3(0.0, 0.5, 1.0);   // Water
        case 3u: return vec3(0.5, 0.5, 0.5);   // Stone
        case 4u: return vec3(0.6, 0.3, 0.1);   // Wood
        case 5u: return vec3(1.0, 0.5, 0.0);   // Fire
        case 6u: return vec3(0.0, 1.0, 0.0);   // Acid
        default: return vec3(1.0, 1.0, 1.0);   // Empty
    }
}

void main() {
    color = vec4(get_material_color(fs_in.material), 1.0);
}
)";

Renderer::Renderer(uint32_t width, uint32_t height)
    : window_(nullptr), gl_context_(nullptr), 
      width_(width), height_(height),
      vao_particles_(0), vbo_positions_(0), vbo_materials_(0),
      program_render_(0) {}

Renderer::~Renderer() {
    if (vao_particles_) glDeleteVertexArrays(1, &vao_particles_);
    if (vbo_positions_) glDeleteBuffers(1, &vbo_positions_);
    if (vbo_materials_) glDeleteBuffers(1, &vbo_materials_);
    if (program_render_) glDeleteProgram(program_render_);
    if (gl_context_) SDL_GL_DeleteContext(gl_context_);
    if (window_) SDL_DestroyWindow(window_);
}

void Renderer::init() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    window_ = SDL_CreateWindow(
        "Hybrid Sand Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_, height_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if (!window_) {
        throw std::runtime_error(std::string("Failed to create SDL window: ") + SDL_GetError());
    }
    
    gl_context_ = SDL_GL_CreateContext(window_);
    if (!gl_context_) {
        throw std::runtime_error(std::string("Failed to create OpenGL context: ") + SDL_GetError());
    }
    
    SDL_GL_MakeCurrent(window_, gl_context_);
    SDL_GL_SetSwapInterval(1); // Vsync
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }
    
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    // Create shader program
    program_render_ = create_program(VERTEX_SHADER, FRAGMENT_SHADER);
    
    // Create VAO
    glGenVertexArrays(1, &vao_particles_);
    glBindVertexArray(vao_particles_);

    glGenBuffers(1, &vbo_positions_);
    glGenBuffers(1, &vbo_materials_);
}

void Renderer::render_particles(const Particle* particles, uint32_t count) {
    if (count == 0) return;
    
    std::vector<glm::vec2> positions;
    std::vector<uint32_t> materials;
    
    positions.reserve(count);
    materials.reserve(count);
    
    for (uint32_t i = 0; i < count; ++i) {
        positions.push_back(particles[i].position);
        materials.push_back(static_cast<uint32_t>(particles[i].material));
    }
    
    glUseProgram(program_render_);
    glBindVertexArray(vao_particles_);

    glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(width_),
        0.0f, static_cast<float>(height_),
        -1.0f, 1.0f
    );
    glUniformMatrix4fv(
        glGetUniformLocation(program_render_, "projection"),
        1,
        GL_FALSE,
        &projection[0][0]
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions_);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(glm::vec2),
        positions.data(),
        GL_STREAM_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_materials_);
    glBufferData(
        GL_ARRAY_BUFFER,
        materials.size() * sizeof(uint32_t),
        materials.data(),
        GL_STREAM_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(uint32_t), nullptr);

    glPointSize(1.0f);
    glDrawArrays(GL_POINTS, 0, count);
}

void Renderer::render_grid(const GridCell* grid, uint32_t width, uint32_t height) {
    // Debug grid rendering
}

void Renderer::render_ui(const PerfStats& stats) {
    // UI rendering
}

void Renderer::present() {
    SDL_GL_SwapWindow(window_);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint Renderer::compile_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Shader compilation failed: " << info_log << std::endl;
    }
    return shader;
}

GLuint Renderer::create_program(const char* vert_src, const char* frag_src) {
    GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    
    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "Program linking failed: " << info_log << std::endl;
    }
    
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}
