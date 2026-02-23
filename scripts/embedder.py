import sys
from pathlib import Path


def main() -> None:
    embed_path = Path(sys.argv[1])
    header_path = Path(sys.argv[2])
    cpp_path = Path(sys.argv[3])

    if not embed_path.exists() or not embed_path.is_dir():
        print(f"Error: Directory '{embed_path}' not found")
        return
    
    files_to_embed = []
    for filepath in embed_path.rglob("*"):
        if filepath.is_file():
            if filepath.stat().st_size == 0:
                # print(f"Skipping entry file: {filepath}")
                continue
            elif ".embed" not in filepath.name:
                continue

            relative_path = filepath.relative_to(embed_path).as_posix()
            clean_relative_path = relative_path.replace(".embed", "")
            full_path = filepath.as_posix()
            files_to_embed.append((clean_relative_path, full_path))
    
    files_to_embed.sort(key=lambda x: x[0])

    header_content = []
    header_content.append("#pragma once")
    header_content.append("#include <cstdint>")
    header_content.append("#include <cstddef>")
    header_content.append("#include <string_view>\n")

    header_content.append("class Resource {")
    header_content.append("private:")
    header_content.append("    const uint8_t* data = nullptr;")
    header_content.append("    size_t size = 0;")
    header_content.append("public:")
    header_content.append("    Resource(std::string_view path);")
    header_content.append("};")

    header_file = "\n".join(header_content)

    cpp_content = []
    cpp_content.append('#include "Resource.hpp"')
    cpp_content.append("#include <array>\n")

    cpp_content.append("namespace {\n")

    for i, (rel_path, full_path) in enumerate(files_to_embed):
        cpp_content.append(f"constexpr unsigned char asset_{i}[] = {{")
        cpp_content.append(f'    #embed "{full_path}"')
        cpp_content.append("};")
    
    cpp_content.append("")
    
    cpp_content.append("struct EmbeddedResource {")
    cpp_content.append("    std::string_view path;")
    cpp_content.append("    const uint8_t* data;")
    cpp_content.append("    size_t size;")
    cpp_content.append("};\n")

    cpp_content.append(f"constexpr std::array<EmbeddedResource, {len(files_to_embed)}> resources = {{{{")

    for i, (rel_path, full_path) in enumerate(files_to_embed):
        cpp_content.append(f'    {{"{rel_path}", asset_{i}, sizeof(asset_{i})}},')
    
    cpp_content.append("}};")
    cpp_content.append("}\n")

    cpp_content.append("Resource::Resource(std::string_view path) {")
    cpp_content.append("    for (const auto& entry : resources) {")
    cpp_content.append("        if (entry.path == path) {")
    cpp_content.append("            data = entry.data;")
    cpp_content.append("            size = entry.size;")
    cpp_content.append("            return;")
    cpp_content.append("        }")
    cpp_content.append("    }")
    cpp_content.append("}")

    cpp_file = "\n".join(cpp_content)

    header_path.write_text(header_file)
    cpp_path.write_text(cpp_file)


if __name__ == "__main__":
    main()
