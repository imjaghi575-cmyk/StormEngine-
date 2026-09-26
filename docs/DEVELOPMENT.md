# Development Guide

## Verification policy

Storm Engine follows a verify-before-claim workflow:

1. Check the current repository state before editing.
2. Check current official documentation for APIs and build tooling.
3. Make small, isolated commits.
4. Add or update tests for behavior changes.
5. Run CTest locally when a native build environment is available.
6. Inspect GitHub Actions results after pushes.
7. Do not claim a build or CI run passed unless its result is observable.

## Native development

The project uses CMake and C++17. A normal local verification cycle is:

```sh
cmake -S . -B build -DSTORM_BUILD_DEMO=ON -DSTORM_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure --no-tests=error
```

For sanitizer verification:

```sh
cmake -S . -B build-sanitized \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSTORM_BUILD_DEMO=OFF \
  -DSTORM_BUILD_TESTS=ON \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"

cmake --build build-sanitized --parallel
ctest --test-dir build-sanitized --output-on-failure --no-tests=error
```

## Architecture rules

- Keep platform-specific code out of Core.
- Keep rendering backends behind stable interfaces.
- Prefer ownership through RAII and smart pointers.
- Keep public headers free of implementation-only dependencies where practical.
- Add regression tests for fixed bugs.
- Prefer small commits that can be independently verified.

## Android

Android integration must be based on current official Android/NDK documentation. Android-specific APIs belong in the Platform layer rather than Core.

## CI

GitHub Actions verifies GCC and Clang Debug/Release builds and a GCC AddressSanitizer/UndefinedBehaviorSanitizer build.

A missing workflow result is not treated as a passing result.
