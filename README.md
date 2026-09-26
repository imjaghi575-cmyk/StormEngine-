# Storm Engine

Storm Engine is a small modular game engine intended for Android/Termux and later an Android native application.

Storm is an independent project. It is not a fork or clone of Godot.

## Current status

- C++17 engine core
- Vec2 and Transform2D
- Node/Scene hierarchy
- Frame timing and update loop
- Platform abstraction and headless backend
- Input state and queued keyboard/pointer events
- Deterministic fixed-timestep update support
- Renderer2D abstraction with deterministic software texture drawing
- Software renderer test backend with deterministic in-memory framebuffer
- Seven CTest core/platform/renderer/time/input/fixed-timestep/resource test targets
- Resource abstraction with deterministic memory and bounded file providers
- GCC and Clang Debug/Release CI
- GCC AddressSanitizer/UndefinedBehaviorSanitizer CI
- CMake Debug/Release/Android arm64 presets using Ninja
- Scene traversal protected against sibling additions during an update pass

The software renderer is intentionally a test backend. It does not create a graphical window.

## Requirements

For native Termux builds: Clang/C++17, CMake, Ninja, and standard build tools.

```sh
pkg update
pkg install clang cmake ninja
```

For other platforms, use a CMake version that satisfies the project's minimum version (3.23) and a supported C++17 compiler.

## Build and test

A direct CMake build:

```sh
cmake -S . -B build -G Ninja -DSTORM_BUILD_DEMO=ON -DSTORM_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure --no-tests=error
```

With CMake presets:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Release:

```sh
cmake --preset release
cmake --build --preset release
ctest --preset release
```

The test executables keep assertions enabled even when the engine itself is built as Release, so Release CI does not silently skip assertion-based checks.

Optional installation exports a relocatable `StormEngine::storm_core` CMake package. CI also builds a small external consumer against the installed package.

## Development rules

1. Keep platform-specific code outside the engine core.
2. Keep rendering backends behind interfaces.
3. Add tests when core behavior changes.
4. Prefer small, reviewable commits.
5. Never claim an Android, Termux, or CI build passed unless its result was actually observed.
6. Verify platform APIs against current official documentation before implementation.
7. Treat a missing CI result as unknown, not as success.

## Architecture

```text
Storm Engine
├── Core
├── Platform
│   ├── Headless
│   └── Android (planned)
├── Renderer
│   ├── Software
│   ├── OpenGL ES (planned)
│   └── Vulkan (planned)
├── Resources
│   ├── Memory
│   └── Bounded File
├── Audio (planned)
└── Editor (planned)
```

## Android direction

Android integration will remain separate from the engine core. The native application layer will use Android-supported native APIs together with the NDK and CMake. The exact integration will be implemented only after checking current official Android documentation.

## License

MIT. See LICENSE.