# Recreating-DOS2-SurfaceSystem

A C++ / raylib experiment recreating the surface system from *Dungeon of the Endless 2*:
a ball that moves across a 2D surface.

## Requirements

- CMake 3.24+
- Visual Studio 2022 (MSVC, x64)
- raylib — optional. If not installed, CMake downloads and builds raylib 6.2 on the
  first configure. This is handled automatically; you only need network access once.

## Build

Using CMake presets:

```sh
cmake --preset windows-x64
cmake --build --preset debug
```

Or open the folder directly in Visual Studio 2022 ("Open a local folder"), which picks up
`CMakePresets.json` and sets `Recreating-DOS2-SurfaceSystem` as the startup project.

The binary is written to `out/build/windows-x64/<Config>/`.

## Layout

| Path | Purpose |
| --- | --- |
| `CMakeLists.txt` | Build definition, raylib wiring |
| `CMakePresets.json` | `windows-x64` configure preset, `debug` / `release` build presets |
| `src/main.cpp` | Entry point and main loop |
| `src/ball.cpp`, `src/ball.h` | Ball state and movement |
