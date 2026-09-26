# Storm Engine Architecture

## Design goals

Storm Engine keeps the engine core independent from Android and desktop windowing APIs. Platform and renderer implementations are selected behind small interfaces so the core can remain usable from Termux, native Android code, and headless CI.

## Core layers

### Core

`storm/core.hpp` and `src/storm/core.cpp` provide:

- `Node` and `Scene` ownership through `std::unique_ptr`.
- `Transform2D` and `Vec2`.
- `Engine` scene ownership, input processing, variable updates, and fixed updates.

A scene update snapshots the number of children before traversing them. A child that adds a sibling during its update therefore does not cause that new sibling to execute unexpectedly in the same parent traversal.

### Time

`FrameClock` clamps variable-frame deltas to the configured maximum.

`FixedTimestep` accumulates elapsed time and executes deterministic fixed-size updates up to a configurable per-call limit. When the limit is reached, only the fractional remainder is retained instead of silently discarding all remaining time. Invalid or non-finite elapsed values are ignored.

### Input

`InputQueue` stores ordered input events. `InputState` converts those events into current keyboard and pointer state.

Unknown keyboard and pointer sentinel values are deliberately not reported as pressed.

### Platform

`Platform` is the boundary for window/input integration. The current headless implementation exists for CI and engine-core tests. Android-specific native integration is intentionally outside the core.

### Rendering

`Renderer2D` is the renderer interface. The current software renderer is a deterministic test backend rather than a windowing system.

Its framebuffer is stored as a contiguous `Color` array. `begin_frame` clears it, and `draw_rect` performs finite-rectangle validation and viewport clipping. Texture loading is not implemented yet; `draw_sprite` remains a no-op until the resource system exists.

### Resources

The resource layer currently provides a small `Resource` value type and `ResourceProvider` interface. `MemoryResourceProvider` supplies deterministic test data without filesystem or Android dependencies. `FileResourceProvider` is also available for bounded filesystem loading. It canonicalizes the provider root and rejects absolute paths and paths that resolve outside that root. Asynchronous I/O and caching are intentionally not part of this first synchronous API.

## Build and test model

The repository uses CMake presets and CTest. CI verifies:

1. GCC Debug
2. GCC Release
3. Clang Debug
4. Clang Release
5. GCC AddressSanitizer + UndefinedBehaviorSanitizer
6. Android NDK arm64-v8a

The Android build uses the NDK CMake toolchain and does not build desktop demos or tests.

## Development roadmap

The next engine layers should be implemented in this order:

1. Image/pixel-format resources that can feed renderer backends.
2. A real Android platform/application layer using NDK-supported APIs.
3. OpenGL ES rendering behind `Renderer2D`.
4. Audio abstraction and an Android backend.
5. Serialization and scene/resource loading.
6. Profiling/diagnostics and broader integration tests.
7. Tooling/editor functionality only after the runtime APIs stabilize.


Each layer should add tests before it becomes a dependency of another layer.
