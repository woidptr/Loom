import sys
from jinja2 import Template
from dataclasses import dataclass
from pathlib import Path


@dataclass
class ResourceFiles:
    header: Path
    source: Path


@dataclass
class ResourceFilesContent:
    header: str
    source: str


def collect_files_to_embed(embed_path: Path) -> list[tuple[str, str]]:
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

    return files_to_embed


def generate_files_content(embed_path: Path, resource_files: ResourceFiles, files_to_embed: list) -> ResourceFilesContent:
    header_template_path = embed_path / "scripts" / "templates" / "Resource.hpp.template"
    source_template_path = embed_path / "scripts" / "templates" / "Resource.cpp.template"

    header_template_text = header_template_path.read_text()
    source_template_text = source_template_path.read_text()

    source_template = Template(source_template_text)
    
    source_result = source_template.render(files=files_to_embed)

    files_content = ResourceFilesContent(
        header=header_template_text,
        source=source_result,
    )

    return files_content


def write_files_safely(resource_files: ResourceFiles, files_content: ResourceFilesContent) -> None:
    original_header_content = resource_files.header.read_text()

    if original_header_content != files_content.header:
        resource_files.header.write_text(files_content.header)
    
    original_source_content = resource_files.source.read_text()

    if original_source_content != files_content.source:
        resource_files.source.write_text(files_content.source)


def main() -> None:
    embed_path = Path(sys.argv[1])
    header_path = Path(sys.argv[2])
    cpp_path = Path(sys.argv[3])

    resource_files = ResourceFiles(
        header=header_path,
        source=cpp_path,
    )

    header_path.parent.mkdir(parents=True, exist_ok=True)

    files_to_embed = collect_files_to_embed(embed_path)

    files_content = generate_files_content(embed_path, resource_files, files_to_embed)

    write_files_safely(resource_files, files_content)


if __name__ == "__main__":
    main()
