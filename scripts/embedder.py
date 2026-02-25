import sys
from pathlib import Path


def main() -> None:
    embed_path = Path(sys.argv[1])
    header_path = Path(sys.argv[2])
    cpp_path = Path(sys.argv[3])

    header_path.parent.mkdir(parents=True, exist_ok=True)

    files_to_embed = []
    for filepath in embed_path.rglob("*"):
        if filepath.is_file():
            if filepath.stat().st_size == 0:
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
    header_content.append("#include <string_view>")
    header_content.append("#include <libhat/fixed_string.hpp>")
    header_content.append("#include <sdk/mc/deps/core/string/HashedString.hpp>\n")

    header_content.append("struct HashedPath {")
    header_content.append("    uint64_t value;\n")
    header_content.append("    consteval HashedPath(const char* path)")
    header_content.append("        : value(HashedString::computeHash(std::string_view{path})) {}\n")
    header_content.append("    consteval HashedPath(std::string_view path)")
    header_content.append("        : value(HashedString::computeHash(path)) {}")
    header_content.append("};\n")

    header_content.append("class Resource {")
    header_content.append("private:")
    header_content.append("    const uint8_t* m_data = nullptr;")
    header_content.append("    size_t m_size = 0;")
    header_content.append("public:")
    header_content.append("    Resource(HashedPath hashed_path);\n")
    header_content.append("    const uint8_t* data() const;")
    header_content.append("    size_t size() const;")
    header_content.append("    std::string_view str() const;")
    header_content.append("};")

    header_file = "\n".join(header_content)

    cpp_content = []
    cpp_content.append('#include "Resource.hpp"')
    cpp_content.append("#include <array>\n")

    cpp_content.append("namespace {\n")

    for i, (rel_path, full_path) in enumerate(files_to_embed):
        cpp_content.append(f"constexpr uint8_t asset_{i}[] = {{")
        cpp_content.append(f'    #embed "{full_path}"')
        cpp_content.append("};")
    
    cpp_content.append("")
    
    cpp_content.append("struct EmbeddedResource {")
    cpp_content.append("    uint64_t path_hash;")
    cpp_content.append("    const uint8_t* data;")
    cpp_content.append("    size_t size;")
    cpp_content.append("};\n")

    cpp_content.append(f"constexpr std::array<EmbeddedResource, {len(files_to_embed)}> resources = {{{{")

    for i, (rel_path, full_path) in enumerate(files_to_embed):
        cpp_content.append(f'    {{HashedString::computeHash("{rel_path}"), asset_{i}, sizeof(asset_{i})}},')
    
    cpp_content.append("}};")
    cpp_content.append("}\n")

    cpp_content.append("Resource::Resource(HashedPath hashed_path) {")
    cpp_content.append("    for (const auto& entry : resources) {")
    cpp_content.append("        if (entry.path_hash == hashed_path.value) {")
    cpp_content.append("            m_data = entry.data;")
    cpp_content.append("            m_size = entry.size;")
    cpp_content.append("            return;")
    cpp_content.append("        }")
    cpp_content.append("    }")
    cpp_content.append("}\n")

    cpp_content.append("const uint8_t* Resource::data() const {")
    cpp_content.append("    return m_data;")
    cpp_content.append("}\n")

    cpp_content.append("size_t Resource::size() const {")
    cpp_content.append("    return m_size;")
    cpp_content.append("}\n")

    cpp_content.append("std::string_view Resource::str() const {")
    cpp_content.append("    return std::string_view(reinterpret_cast<const char*>(m_data), m_size);")
    cpp_content.append("}")

    cpp_file = "\n".join(cpp_content)

    header_path.write_text(header_file)
    cpp_path.write_text(cpp_file)


if __name__ == "__main__":
    main()
