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

## Logs

When compiled in debug mode and injected, you will see the console pop up and all the information will be printed there.
When compiled in release mode, all the logs are stored at `%appdata%/.loom/logs`.
