# Loom

A playground for testing Minecraft Bedrock Edition native modding

## Requirements

- Microsoft Visual Studio Build Tools
- clang-cl compiler
- CMake
- Python

## Signatures
Signatures are not included in the source. You can find them yourself.
They are loaded from the file `signatures.json` in the root directory.
Rename the current `signatures.json.example` to `signatures.json` and fill in the blank spaces with the corresponding signatures.

## Building

### Debug
```sh
cmake --preset x64-debug-clang-cl
cmake --build --preset x64-debug-clang-cl
```

### Release
```sh
cmake --preset x64-release-clang-cl
cmake --build --preset x64-release-clang-cl
```

## Logs

Logs are located at the `%appdata%/.loom/logs`. In debug mode, the debug console will be available to you.
