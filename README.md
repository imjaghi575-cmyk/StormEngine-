# Storm Engine

Storm Engine is a small modular game engine intended for Android/Termux and later an Android native application.

Storm is an independent project. It is not a fork or clone of Godot.

## Current status

- C++17 engine core
- Vec2 and Transform2D
- Node/Scene hierarchy
- Frame timing and update loop
- Platform abstraction and headless backend
- Input state abstraction
- Renderer2D abstraction
- Software renderer smoke-test backend
- CTest core/platform tests
- GitHub Actions CI
- CMake Debug/Release presets

The software renderer is intentionally a test backend. It does not create a graphical window.

## Requirements

For native Termux builds: Clang/C++17, CMake, and Make or another supported generator.

```sh
pkg update
pkg install clang cmake make
```

## Build and test

```sh
cmake -S . -B build -DSTORM_BUILD_DEMO=ON -DSTORM_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

With CMake presets:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Development rules

1. Keep platform-specific code outside the engine core.
2. Keep rendering backends behind interfaces.
3. Add tests when core behavior changes.
4. Prefer small, reviewable commits.
5. Never claim an Android or Termux build passed unless it was actually run.
6. Verify platform APIs against current official documentation before implementation.

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
├── Resources (planned)
├── Audio (planned)
└── Editor (planned)
```

## Android direction

Android integration will remain separate from the engine core. The native application layer will use Android-supported native APIs together with the NDK and CMake. The exact integration will be implemented only after checking current official Android documentation.

## License

MIT. See LICENSE.
