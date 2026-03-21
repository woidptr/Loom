import sys
from pathlib import Path

from builder.utils.context import ResourceGeneratorContext, ClangParserContext
from builder.scripts.embedder import embed
from builder.scripts.sdk_parser import generate_sdk_metadata


def main() -> None:
    current_source_dir: Path = Path(sys.argv[1])
    current_binary_dir: Path = Path(sys.argv[2])

    templates_dir: Path = current_source_dir / "builder" / "templates"

    embed(ResourceGeneratorContext(
        template_source_path=templates_dir / "Resource.cpp.template",
        result_source_path=current_source_dir / "src" / "core" / "Resource.cpp",
        embed_path=current_source_dir,
    ))

    generate_sdk_metadata(ClangParserContext(
        source_dir=current_source_dir,
        sdk_path=current_source_dir / "src" / "sdk",
        compile_commands_path=current_binary_dir / "compile_commands.json",
        template_import_path=templates_dir / "import.def.template",
        result_import_path=current_binary_dir,
        template_signatures_path=templates_dir / "Signatures.cpp.template",
        result_signatures_path=current_source_dir / "src" / "core" / "Signatures.cpp",
    ))


if __name__ == "__main__":
    main()
