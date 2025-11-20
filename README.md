# Loom

A playground for testing Minecraft Bedrock Edition native modding

## Requirements

- Microsoft Visual Studio Build Tools
- CMake

## Building

Build with Visual Studio by pressing `Ctrl+Shift+B` or can build with cmake commands (don't forget to set the msvc compiler path)

### Debug
```sh
cmake --preset x64-debug
cmake --build --preset x64-debug
```

### Release
```sh
cmake --preset x64-release
cmake --build --preset x64-release
```
