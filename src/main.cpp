#include "engine.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Engine engine;
        engine.init();
        return engine.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
