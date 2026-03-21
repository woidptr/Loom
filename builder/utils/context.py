from dataclasses import dataclass
from pathlib import Path

from builder.utils.enums import SignatureType


@dataclass(frozen=True)
class ExtractedSignature:
    name: str
    pattern: str
    type: SignatureType


@dataclass(frozen=True)
class ClangParserContext:
    source_dir: Path
    sdk_path: Path
    compile_commands_path: Path
    template_import_path: Path
    result_import_path: Path
    template_signatures_path: Path
    result_signatures_path: Path


@dataclass(frozen=True)
class ResourceGeneratorContext:
    template_source_path: Path
    result_source_path: Path
    embed_path: Path
