import re
from pathlib import Path
from dataclasses import dataclass


@dataclass
class ResourceInfo:
    name: str
    path: str


def make_identifier(path: Path) -> str:
    name: str = path.as_posix().lower()

    name: str = re.sub(r"[^a-z0-9]", "_", name)

    if name and name[0].isdigit():
        name = "_" + name
    
    return name


def collect_resources(root: Path) -> list[ResourceInfo]:
    root = root.resolve()
    resources: list[ResourceInfo] = []

    for path in sorted(root.rglob("*")):
        if path.is_file():
            rel = path.relative_to(root)
            identifier = make_identifier(rel)
            resources.append(ResourceInfo(identifier, path.resolve().as_posix()))
    
    return resources


def generate_header(resources: list[ResourceInfo]) -> str:
    lines = []
    lines.append("#pragma once")
    lines.append("#include <cstddef>\n\n")

    for r in resources:
        lines.append(f"constexpr unsigned char {r.name}[] = {{")
        lines.append(f'    #embed "{r.path}"')
        lines.append("};\n")
    
    return "\n".join(lines)


def main() -> None:
    root = Path("../assets")
    resources = collect_resources(root)

    header = generate_header(resources)

    output_path = Path("../resources.hpp")
    output_path.write_text(header, encoding="utf-8")


if __name__ == "__main__":
    main()
