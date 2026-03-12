from jinja2 import Template
from dataclasses import dataclass
from pathlib import Path

from builder.utils.safewrite import write_to_file_safely


@dataclass(frozen=True)
class ResourceFiles:
    header: Path
    source: Path


@dataclass(frozen=True)
class ResourceFilesContent:
    header: str
    source: str


def collect_files_to_embed(embed_path: Path) -> list[tuple[str, str]]:
    files_to_embed: list[str, str] = []
    for filepath in embed_path.rglob("*"):
        if filepath.is_file():
            if filepath.stat().st_size == 0:
                continue
            elif ".embed" not in filepath.name:
                continue

            relative_path: str = filepath.relative_to(embed_path).as_posix()
            clean_relative_path: str = relative_path.replace(".embed", "")
            full_path: str = filepath.as_posix()
            files_to_embed.append((clean_relative_path, full_path))
    
    files_to_embed.sort(key=lambda x: x[0])

    return files_to_embed


def generate_files_content(embed_path: Path, resource_files: ResourceFiles, files_to_embed: list) -> ResourceFilesContent:
    header_template_path: Path = embed_path / "builder" / "templates" / "Resource.hpp.template"
    source_template_path: Path = embed_path / "builder" / "templates" / "Resource.cpp.template"

    header_template_text: str = header_template_path.read_text()
    source_template_text: str = source_template_path.read_text()

    source_template: Template = Template(source_template_text)
    
    source_result: str = source_template.render(files=files_to_embed)

    files_content: ResourceFilesContent = ResourceFilesContent(
        header=header_template_text,
        source=source_result,
    )

    return files_content


def embed(current_source_dir: Path, current_binary_dir: Path) -> None:
    embed_path = current_source_dir

    header_path: Path = current_binary_dir / "embedder" / "Resource.hpp"
    cpp_path: Path = current_binary_dir / "embedder" / "Resource.cpp"

    resource_files = ResourceFiles(
        header=header_path,
        source=cpp_path,
    )

    header_path.parent.mkdir(parents=True, exist_ok=True)

    files_to_embed: list[tuple[str, str]] = collect_files_to_embed(embed_path)

    files_content: ResourceFilesContent = generate_files_content(embed_path, resource_files, files_to_embed)

    write_to_file_safely(resource_files.header, files_content.header)
    write_to_file_safely(resource_files.source, files_content.source)
