# Contributing to Storm Engine

## Development principles

- Keep the engine modular and platform-independent where possible.
- Prefer small, testable changes.
- Do not add an Android-specific dependency to the core unless it is necessary.
- Keep rendering backends behind interfaces.
- Document public APIs and behavior.
- Add or update tests when changing core behavior.

## Local build

```sh
cmake -S . -B build -DSTORM_BUILD_TESTS=ON
cmake --build build -j2
ctest --test-dir build --output-on-failure
```

## Android direction

Android native integration should use the NDK and CMake rather than mixing platform code into the core library. The Android layer will be added separately from the engine core.

## Commit style

Use short conventional prefixes such as:

- feat:
- fix:
- build:
- test:
- docs:
- refactor:
