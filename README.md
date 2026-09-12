# Hybrid Sand Engine

A high-performance falling sand simulation engine targeting 60+ FPS with 10M+ particles.

## Architecture

### CPU/GPU Hybrid System
- **CPU-based physics** (CTPL thread pool) for precise collision detection and material interactions
- **GPU compute shaders** for parallel particle updates at massive scales
- **Adaptive switching**: CPU for <100K particles, GPU for >100K particles
- **Grid-based spatial acceleration** for efficient collision queries

### Components

1. **ParticleSystem** - Manages particle state and dispatching
2. **GPUCompute** - Compute shader pipeline for particle simulation
3. **ThreadPool** - CTPL-based thread pool for CPU physics
4. **World** - Game world state and entity management
5. **Renderer** - OpenGL 4.3+ rendering pipeline
6. **Physics** - Material properties and collision response

### Repository Boundaries

The CMake project is split into two targets so the code can be moved into separate repositories without changing its public boundary:

- `HybridSandCore` - world state, particles, physics, GPU compute, threading, rendering modules, and shared brush operations.
- `HybridSandEngine` - the interactive sandbox executable, input handling, and application loop.

The future layout can use `HybridSandCore` as a library repository and keep the current executable, brush controls, and future editor tools in a separate sandbox/tools repository. The sandbox should link the core target instead of copying simulation code.

To verify the reusable boundary locally:

```powershell
cmake -S . -B build-mingw
cmake --build build-mingw
ctest --test-dir build-mingw --output-on-failure
```

### Brush Controls

- Hold the left mouse button to paint sand.
- Hold the right mouse button to erase particles.
- Press Escape to quit.

## Performance Targets

- **10M+ particles** at 60 FPS
- **Grid resolution** 1024x1024
- **Multi-threaded CPU** physics with 8-thread pool
- **GPU compute** shader pipeline (4.3+)

---

# Complete Installation Guide (VS Code)

This guide walks you through setting up HybridSandEngine from absolute zero to running in VS Code.

## Prerequisites

### What You'll Need
- A Windows PC (or Linux/macOS with minor adjustments)
- ~2 GB disk space
- A graphics card supporting OpenGL 4.3+ (nearly all modern GPUs)
- Administrator access to install software

### Time Required
- **Windows**: ~45 minutes
- **Linux/macOS**: ~30 minutes

---

## Step 1: Install VS Code

### 1.1 Download VS Code
1. Go to https://code.visualstudio.com/
2. Click **"Download for Windows"**
3. Run the installer (`VSCodeUserSetup-x64-*.exe`)
4. Click through the install wizard (accept defaults)
5. Launch VS Code

### 1.2 Install Essential Extensions
Open VS Code and install these extensions from the **Extensions** tab (Ctrl+Shift+X):

1. **C/C++ Extension Pack** (by Microsoft) - Search "C++" and install the pack
   - This includes IntelliSense, debugging, and code navigation
2. **CMake Tools** (by Microsoft)
   - Adds CMake integration to VS Code
3. **CMake** (by twxs)
   - Syntax highlighting for CMakeLists.txt
4. **CodeLLDB** (by Vadim Chugunov)
   - Debugger for stepping through code

After installing, **restart VS Code** (Ctrl+Shift+P → "Developer: Reload Window").

---

## Step 2: Install Compiler & Build Tools

### Option A: Windows (Recommended)

#### 2A.1 Install Visual Studio Build Tools
1. Go to https://visualstudio.microsoft.com/downloads/
2. Scroll down and click **"Tools for Visual Studio 2022"** → **"Build Tools for Visual Studio 2022"**
3. Run the installer
4. Select **"Desktop development with C++"** workload
5. Click **"Install"** (this will take ~15-20 minutes)
6. After installation completes, restart your PC

#### 2A.2 Verify Installation
Open **Command Prompt** and type:
```cmd
cl.exe
```
You should see a message showing the compiler version (not "command not found").

### Option B: Windows (Alternative - MinGW)

1. Go to https://www.mingw-w64.org/downloads/
2. Download **"Online Installer"**
3. Run the installer and select:
   - Architecture: x86_64
   - Threads: posix
   - Exception: seh
4. Install to: `C:\mingw-w64`
5. Add to PATH: 
   - Right-click **"This PC"** → **"Properties"** → **"Advanced system settings"**
   - Click **"Environment Variables"**
   - Under **"User variables"**, select **"Path"** → **"Edit"**
   - Click **"New"** and add: `C:\mingw-w64\bin`
   - Click **"OK"** three times

#### Verify Installation
Open **Command Prompt**:
```cmd
g++ --version
```
Should show version info.

### Option C: Linux

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake git

# Fedora/RHEL
sudo dnf install gcc gcc-c++ cmake git

# Verify
gcc --version
cmake --version
```

### Option D: macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake

# Verify
clang --version
cmake --version
```

---

## Step 3: Install CMake

CMake is a build system generator that creates project files for your compiler.

### Windows
1. Go to https://cmake.org/download/
2. Download **"Windows x64 Installer"**
3. Run the installer and select **"Add CMake to the system PATH for all users"**
4. Complete installation

**Verify:**
```cmd
cmake --version
```

### Linux/macOS
Already installed via package manager (see Step 2).

---

## Step 4: Install Dependencies

Dependencies are libraries the engine needs. We'll use **vcpkg** (package manager) to install them automatically.

### 4.1 Install vcpkg

#### Windows
1. Open **Command Prompt**
2. Navigate to a folder where you want to install vcpkg (e.g., `C:\dev`):
```cmd
mkdir C:\dev
cd C:\dev
```

3. Clone vcpkg:
```cmd
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
```

4. Run the bootstrap script:
```cmd
.\bootstrap-vcpkg.bat
```

This creates `vcpkg.exe`.

#### Linux/macOS
```bash
mkdir ~/dev
cd ~/dev
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

### 4.2 Install Required Libraries

From the vcpkg directory, install the dependencies:

#### Windows (Visual Studio)
```cmd
.\vcpkg install sdl2:x64-windows glew:x64-windows glm:x64-windows
```

#### Windows (MinGW)
```cmd
 .\vcpkg install sdl2:x64-mingw-dynamic glew:x64-mingw-dynamic glm:x64-mingw-dynamic
```

#### Linux/macOS
```bash
./vcpkg install sdl2:x64-linux glew:x64-linux glm:x64-linux
```

This will take **5-10 minutes** as it downloads and compiles each library.

---

## Step 5: Clone the Repository

### 5.1 Clone HybridSandEngine

Open **Command Prompt** (Windows) or **Terminal** (Linux/macOS) and navigate to your dev folder:

#### Windows
```cmd
cd C:\dev
git clone https://github.com/Aireos/HybridSandEngine.git
cd HybridSandEngine
```

#### Linux/macOS
```bash
cd ~/dev
git clone https://github.com/Aireos/HybridSandEngine.git
cd HybridSandEngine
```

---

## Step 6: Create VS Code Workspace

### 6.1 Open HybridSandEngine in VS Code

#### Windows
```cmd
code .
```

#### Linux/macOS
```bash
code .
```

This opens the project folder in VS Code.

### 6.2 Create `.vscode` Folder with Configuration

In VS Code, create a `.vscode` folder in the project root if it doesn't exist.

**File** → **New Folder** → Name it `.vscode`

### 6.3 Create `settings.json`

Right-click `.vscode` → **New File** → Name it `settings.json`

Paste this content (replace paths with your vcpkg path):

#### Windows (Visual Studio)
```json
{
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-windows"
    },
    "cmake.preferredGenerators": [
        "Visual Studio 17 2022"
    ],
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### Windows (MinGW)
```json
{
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-mingw-dynamic",
        "CMAKE_MAKE_PROGRAM": "C:/msys64/usr/bin/make.exe"
    },
    "cmake.preferredGenerators": [
        "MSYS Makefiles"
    ],
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### Linux
```json
{
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "/home/USERNAME/dev/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-linux"
    },
    "cmake.preferredGenerators": [
        "Unix Makefiles"
    ],
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### macOS
```json
{
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "/Users/USERNAME/dev/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-osx"
    },
    "cmake.preferredGenerators": [
        "Unix Makefiles"
    ],
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

**Save the file** (Ctrl+S).

### 6.4 Create `launch.json` (Debugger)

Right-click `.vscode` → **New File** → Name it `launch.json`

Paste this content:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug HybridSandEngine",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/Debug/HybridSandEngine",
            "args": [],
            "stopOnEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "lldb",
            "preLaunchTask": "CMake: build"
        }
    ]
}
```

**Save** (Ctrl+S).

---

## Step 7: Configure CMake

### 7.1 Open CMake Configuration

Press **Ctrl+Shift+P** to open the Command Palette and type:
```
CMake: Configure
```

VS Code will prompt you to select a generator:
- **Windows (Visual Studio)**: Select `Visual Studio 17 2022`
- **Windows (MinGW)**: Select `MinGW Makefiles`
- **Linux/macOS**: Select `Unix Makefiles`

Press **Enter**. This will configure the project and generate build files. **Wait for it to complete** (~2-3 minutes).

### 7.2 Verify Configuration

At the bottom of VS Code, you should see:
```
[CMake] Build folder: [project]/build
[CMake] Configuration done
```

If you see errors, check:
1. vcpkg path in `settings.json` is correct
2. All dependencies installed successfully
3. Compiler is installed and in PATH

---

## Step 8: Build the Project

### 8.1 Build

Press **Ctrl+Shift+P** and type:
```
CMake: Build
```

Press **Enter**. VS Code will compile the project. **Wait 3-5 minutes** for compilation to complete.

Watch the **Output** panel for any errors.

### 8.2 Verify Build Success

Look for:
```
[CMake] Build complete
[CMake] Build folder: [project]/build
```

The executable should be at:
- **Windows**: `build\Debug\HybridSandEngine.exe`
- **Linux/macOS**: `build/Debug/HybridSandEngine`

---

## Step 9: Run the Engine

### 9.1 Run from VS Code

Press **Ctrl+Shift+P** and type:
```
CMake: Run
```

Or use the **Play button** at the bottom of VS Code.

### 9.2 What You Should See

A window opens showing:
- A simulation of sand falling under gravity
- Console output showing FPS, particle count, and timing
- Press **ESC** to quit

**Congratulations! You're running the engine!** 🎉

---

## Step 10: Start Developing

### Project Structure

```
HybridSandEngine/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── common.hpp          # Constants and structs
│   ├── engine.hpp          # Main engine class
│   ├── world.hpp           # World simulation
│   ├── particle_system.hpp # Particle management
│   ├── gpu_compute.hpp     # GPU shaders
│   ├── physics.hpp         # Physics calculations
│   ├── renderer.hpp        # Graphics rendering
│   └── thread_pool.hpp     # Thread management
├── src/                    # Source files
│   ├── main.cpp            # Entry point
│   ├── engine.cpp
│   ├── world.cpp
│   ├── particle_system.cpp
│   ├── gpu_compute.cpp
│   ├── physics.cpp
│   ├── renderer.cpp
│   └── thread_pool.cpp
├── build/                  # Generated build files (auto-created)
├── README.md               # This file
└── CMakeLists.txt         # Build system config
```

### Editing Code

1. Open any file in the `include/` or `src/` folders
2. Make your changes
3. **Ctrl+Shift+P** → **CMake: Build**
4. **Ctrl+Shift+P** → **CMake: Run**

### Debugging

1. Set a breakpoint by clicking to the left of a line number (red dot appears)
2. **F5** or **Ctrl+Shift+P** → **Debug: Start Debugging**
3. Use the debug controls to step through code

---

## Troubleshooting

### Issue: "cmake: command not found"
**Solution:** CMake isn't installed or not in PATH.
- Reinstall CMake and select "Add to PATH"
- Or add manually: Right-click PC → Properties → Environment Variables → Add CMake path

### Issue: "SDL2 not found"
**Solution:** vcpkg didn't install dependencies.
1. Check vcpkg path in `settings.json`
2. Run vcpkg install again:
```cmd
C:\dev\vcpkg\vcpkg install sdl2:x64-windows glew:x64-windows glm:x64-windows
```

### Issue: "No compiler found"
**Solution:** C++ compiler not installed.
- **Windows**: Install Visual Studio Build Tools (Step 2)
- **Linux**: `sudo apt install build-essential`
- **macOS**: `xcode-select --install`

### Issue: "No OpenGL 4.3 support"
**Solution:** Your GPU doesn't support OpenGL 4.3.
- Downgrade the engine to OpenGL 3.3 (edit `gpu_compute.hpp`)
- Or use a better graphics card

### Issue: Build fails with linking errors
**Solution:** vcpkg libraries not found.
1. Delete `build/` folder
2. Re-run CMake Configure (Step 7.1)
3. Clean rebuild: **Ctrl+Shift+P** → **CMake: Clean**

---

## Next Steps

### Extend the Engine
1. Modify `src/engine.cpp` to add custom input handling
2. Edit `src/particle_system.cpp` to add new material types
3. Tweak `include/common.hpp` constants to change simulation behavior

### Add Features
- Destruction tools: Modify `world.destroy_circle()`
- Material reactions: Add to `particle_system.cpp::handle_material_reactions()`
- Custom rendering: Edit `src/renderer.cpp`

### Performance Tuning
- Increase `MAX_PARTICLES` in `include/common.hpp`
- Adjust thread pool size: `THREAD_POOL_SIZE`
- Change GPU batch size in `gpu_compute.hpp`

---

## Performance Targets

- **10M+ particles** at 60 FPS
- **Grid resolution** 1024x1024
- **Multi-threaded CPU** physics with 8-thread pool
- **GPU compute** shader pipeline (4.3+)

## Materials

- **Sand** (1600 kg/m³, 0.3 friction)
- **Water** (1000 kg/m³, 0.5 friction)
- **Stone** (2500 kg/m³, 0.2 friction)
- **Wood** (600 kg/m³, 0.4 friction)
- **Fire** (reactive)
- **Acid** (reactive)

## Optimization Notes

### CPU Optimizations
- 32-byte particle struct (cache-aligned)
- Thread pool with lock-free queuing
- Spatial grid for collision queries
- SIMD-friendly data layout

### GPU Optimizations
- Compute shader local size 256 (optimal for modern GPUs)
- Persistent buffer mapping for zero-copy uploads
- Workgroup memory for particle interactions
- Memory barriers minimized

## Future Improvements

- [ ] Material reactions (water/fire interaction)
- [ ] Particle pooling and recycling
- [ ] Adaptive LOD based on density
- [ ] Persistent boundary conditions
- [ ] Pressure-based flow simulation
- [ ] Destruction/building tools
- [ ] Save/load world state
- [ ] Network multiplayer support

## License

MIT
