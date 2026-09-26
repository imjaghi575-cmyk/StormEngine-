# Building StormEngine

## Termux

StormEngine is designed to build as a standard CMake C++ project in Termux.

Install the required packages:

    pkg update
    pkg install clang cmake ninja

Configure and build:

    cmake --preset debug
    cmake --build --preset debug
    ctest --preset debug

For a release build:

    cmake --preset release
    cmake --build --preset release
    ctest --preset release

Do not commit generated `build/` directories.

## Android NDK

The Android preset intentionally uses the NDK-provided CMake toolchain rather than CMake's built-in Android platform support.

Set `ANDROID_NDK_HOME` to the installed NDK directory, then run:

    cmake --preset android-arm64
    cmake --build build/android-arm64

The preset targets `arm64-v8a` and Android API 24. The Android application/packaging layer is not part of this preset yet; this preset validates cross-compilation of the engine core.

## CI

GitHub Actions validates GCC and Clang Debug/Release builds and runs CTest. A separate sanitizer job runs AddressSanitizer and UndefinedBehaviorSanitizer.

A CI result is considered authoritative only after the corresponding GitHub Actions run reports success. Local source inspection alone is not treated as proof that the complete repository builds.
