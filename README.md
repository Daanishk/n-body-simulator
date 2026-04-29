
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

## Architecture

The project is layered from low-level GPU resources up to the simulation loop:

```
main.cpp            Simulation loop, force accumulation, integration, render dispatch
  ├── object        Per-body state (position, velocity, mass) + owned VAO/VBO
  ├── Camera        View + projection matrices, keyboard/mouse input
  ├── Shader        GLSL program loading, compilation, linking
  └── VAO/VBO/EBO   RAII wrappers around OpenGL buffer objects
```

Each frame:

1. Pairwise gravitational forces are accumulated for every body.
2. Velocities and positions are integrated forward (explicit Euler).
3. A model matrix is built per body and uploaded as a uniform.
4. The vertex shader transforms object-space vertices into clip space via `projection * view * model`.
5. The fragment shader computes ambient + diffuse lighting from the sun's world position, except for emissive bodies which output their color directly.
6. Trails are rendered as a `GL_LINE_STRIP` of accumulated world positions.

## Math Primitives

The same 3D math primitives that power the rendering pipeline show up across sensor processing and robotics:

- **Coordinate frame transforms** — model and view matrices move geometry between object, world, and camera space. The same 4x4 transforms are used to bring sensor measurements from a sensor frame into a world frame.
- **Vector normalization and dot products** — used in the fragment shader for Lambertian shading (`dot(normal, lightDir)`), and in geometry processing for surface orientation, alignment checks, and projections.
- **Perspective projection** — `glm::perspective` defines a view frustum the same way a camera intrinsic matrix projects 3D points into a 2D image plane.
- **Spherical coordinates** — `buildSphere()` generates vertices from `(θ, φ)`, the same parameterization used to describe an angular sweep of beam directions.
- **Euclidean distance** — pairwise distance computation in the gravity loop is the same primitive underlying nearest-neighbor queries on point sets.

## Limitations & Future Work

Known issues and the upgrades I'd prioritize:

- **Physics is framerate-coupled.** Position and velocity updates use hardcoded scalars instead of a real `dt`, so simulation speed depends on render rate. A fixed-timestep accumulator would make this deterministic.
- **Explicit Euler integrator is unstable for orbits.** Energy drifts over time and orbits decay or escape. Velocity Verlet or RK4 would be a meaningful upgrade.
- **Each body owns its own copy of an identical sphere mesh.** Switching to instanced rendering (`glDrawArraysInstanced`) with a single shared mesh and per-instance model matrices would scale to many more bodies.
- **O(N²) force computation.** Fine for a handful of bodies, but a Barnes-Hut octree would bring this to O(N log N) and make hundreds of bodies feasible.
- **Vertex normals are computed as `normalize(position)`.** This is a sphere-specific shortcut that breaks for arbitrary meshes — proper per-vertex normals should be precomputed and passed as a separate attribute.

## Tech Stack

C++17 · OpenGL 3.3 Core · GLSL · GLFW · GLAD · GLM
