
# Real-Time N-Body Gravity Simulation

A real-time 3D gravitational simulation written in C++ with a custom OpenGL rendering pipeline. Bodies attract one another via Newtonian gravity, integrate forward in time, and are rendered with Lambertian shading from a central emissive sun.

Built from scratch, to learn the graphics pipeline and 3D math primitives end to end.

## Demo Video

[![Watch the demo](assets/thumbnail.png)](https://youtu.be/Yio0VDJi7WM)

## Features

- N-body gravitational simulation with pairwise force accumulation
- Procedural sphere mesh generation in spherical coordinates
- Custom GLSL vertex and fragment shaders with Lambertian diffuse lighting
- Emissive shading flag for self-illuminated bodies (the sun)
- Free-fly 3D camera with mouse look and WASD movement
- World-space trail rendering for orbital paths
- RAII wrappers around OpenGL buffer objects (VAO, VBO, EBO, Shader)

## Controls

| Input | Action |
|---|---|
| `W` `A` `S` `D` | Move forward / left / back / right |
| `Space` / `Left Ctrl` | Move up / down |
| `Left Shift` | Sprint (10x speed) |
| Hold `LMB` + move mouse | Look around |

## Build & Run

Built with Visual Studio 2022, C++17, on Windows x64.

**Dependencies:** GLFW, GLAD, GLM, stb_image (headers vendored in the project).

1. Open `blackhole-simulation.vcxproj` in Visual Studio.
2. Select the `Debug | x64` configuration.
3. Press `Ctrl+F5` to build and run.

The working directory must be the project root so shader files at `Shaders/default.vert` and `Shaders/default.frag` resolve correctly.

## Tech Stack

C++17 · OpenGL 3.3 Core · GLSL · GLFW · GLAD · GLM
