from jinja2 import Template
from dataclasses import dataclass
from pathlib import Path

from builder.utils.context import ResourceGeneratorContext
from builder.utils.safewrite import write_to_file_safely


@dataclass(frozen=True)
class FileContext:
    rel_path: str
    full_path: str


def collect_files_to_embed(embed_path: Path) -> list[FileContext]:
    files_to_embed: list[FileContext] = []
    for filepath in embed_path.rglob("*"):
        if filepath.is_file():
            if filepath.stat().st_size == 0:
                continue
            elif ".embed" not in filepath.name:
                continue

            relative_path: str = filepath.relative_to(embed_path).as_posix()
            clean_relative_path: str = relative_path.replace(".embed", "")
            full_path: str = filepath.as_posix()
            file_context = FileContext(
                rel_path=clean_relative_path,
                full_path=full_path,
            )
            files_to_embed.append(file_context)
    
    files_to_embed.sort(key=lambda x: x.rel_path)

    return files_to_embed


def generate_files_content(template_path: Path, files_to_embed: list) -> str:
    template_text: str = template_path.read_text()
    template: Template = Template(template_text)
    result: str = template.render(files=files_to_embed)

    return result


def embed(ctx: ResourceGeneratorContext) -> None:
    files_to_embed: list[tuple[str, str]] = collect_files_to_embed(ctx.embed_path)

    content: str = generate_files_content(ctx.template_source_path, files_to_embed)

    write_to_file_safely(ctx.result_source_path, content)
