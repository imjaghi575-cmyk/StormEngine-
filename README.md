# Storm Engine

Storm Engine is a small, modular game engine designed to be developed and built on Android/Termux.

## Current direction

The architecture is inspired by the useful ideas behind engines such as Godot:
- Node/Scene based game objects
- A small core library
- Platform-independent math and timing
- A command-line friendly development workflow
- Rendering, input, resources, scripting and an editor added incrementally

This project is **not** a fork or clone of Godot.

## Build on Termux

Install the basic compiler toolchain:

```sh
pkg update
pkg install clang cmake make
```

Build and run the first core demo:

```sh
cmake -S . -B build
cmake --build build -j2
./build/storm_demo
```

## Roadmap

1. Core loop and timing
2. Node and Scene system
3. Math types and transforms
4. Resource system
5. 2D renderer
6. Input system
7. Audio
8. 3D renderer
9. Scripting
10. Cross-platform editor

## License

MIT
