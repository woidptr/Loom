# Loom

A playground for testing Minecraft Bedrock Edition native modding

## Requirements

- Microsoft Visual Studio Build Tools
- CMake

## Signatures
Signatures are not included in the source. You can find them yourself.
They are loaded from the file `signatures.json` in the root directory.
The contents look like this:
```json
{
    "ClassName::function_name": "byte pattern"
}
```

## Building

Build with Visual Studio by pressing `Ctrl+Shift+B` or can build with cmake commands (don't forget to set the msvc compiler path)

### Debug
```sh
cmake --preset x64-debug-msvc
cmake --build --preset x64-debug-msvc
```

### Release
```sh
cmake --preset x64-release-msvc
cmake --build --preset x64-release-msvc
```

## Logs

When compiled in debug mode and injected, you will see the console pop up and all the information will be printed there.
When compiled in release mode, all the logs are stored at `%appdata%/.loom/logs`.
