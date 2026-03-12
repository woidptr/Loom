import json
from pathlib import Path
from dataclasses import dataclass
from jinja2 import Template
from clang.cindex import Index, CursorKind, Cursor, TranslationUnit

from builder.utils.safewrite import write_to_file_safely


@dataclass(frozen=True)
class ExtractedSymbol:
    qualified_name: str
    mangled_name: str


def get_fully_qualified_name(cursor: Cursor) -> str:
    name: str = cursor.spelling
    parent: Cursor | None = cursor.semantic_parent

    while parent and parent.kind != CursorKind.TRANSLATION_UNIT:
        if parent.spelling:
            name = f"{parent.spelling}::{name}"
        parent = parent.semantic_parent
    
    return name


def parse_functions(cursor: Cursor, seen_symbols: set) -> list[ExtractedSymbol]:
    found_symbols: list[ExtractedSymbol] = []

    # if cursor.location.file:
    #     file_path = str(cursor.location.file.name).replace("\\", "/").lower()

    #     if sdk_dir_str not in file_path and "umbrella.cpp" not in file_path:
    #         return []

    if cursor.kind in (CursorKind.FUNCTION_DECL, CursorKind.CXX_METHOD):
        mcapi_func: bool = False

        for child in cursor.get_children():
            if child.kind.name == "DLLIMPORT_ATTR":
                mcapi_func = True
                break

            if child.kind.name == "UNEXPOSED_ATTR" and "dllimport" in str(child.spelling).lower():
                mcapi_func = True
                break
        
        if mcapi_func:
            mangled = cursor.mangled_name
            if mangled and mangled not in seen_symbols:
                seen_symbols.add(mangled)

                qualified_name: str = get_fully_qualified_name(cursor)

                symbol = ExtractedSymbol(
                    qualified_name=qualified_name,
                    mangled_name=mangled,
                )

                found_symbols.append(symbol)
    
    child_node: Cursor
    for child_node in cursor.get_children():
        found_symbols.extend(parse_functions(child_node, seen_symbols))
    
    return found_symbols


def extract_cmake_flags(build_dir: Path) -> list[str]:
    compdb_path: Path = build_dir / "compile_commands.json"

    with open(compdb_path, "r", encoding="utf-8") as f:
        db = json.load(f)
    
    command = db[0].get("command", "")


def generate_fake_imports(current_source_dir: Path, current_binary_dir: Path) -> None:
    sdk_dir: Path = current_source_dir / "src" / "sdk"
    output_file: Path = current_binary_dir / "import.def"
    umbrella_file: Path = current_binary_dir / "umbrella.cpp"
    import_template_path: Path = current_source_dir / "builder" / "templates" / "import.def.template"

    import_template_text: str = import_template_path.read_text()

    import_template: Template = Template(import_template_text)

    index: Index = Index.create()
    seen_symbols: set[str] = set()

    includes = []
    for path in sdk_dir.rglob("*.hpp"):
        safe_path = path.resolve().as_posix()
        includes.append(f'#include "{safe_path}"')
    
    umbrella_file.write_text("\n".join(includes))

    clang_args = [
        "-x", "c++", 
        "-std=c++23", 
        "-fms-extensions",
        
        # 1. Include paths: Tell Clang where your 'src', 'sdk', and 'core' folders live
        f"-I{current_source_dir / 'src'}",
        
        # 2. Bypass MSVC's aggressive compiler version check
        "-D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH",
        
        # 3. Silence the #pragma once warning
        "-Wno-pragma-once-outside-header"
    ]

    tu: TranslationUnit = index.parse(str(umbrella_file), args=clang_args)

    for diag in tu.diagnostics:
        print(f"  Warning/Error: {diag.spelling}")
    
    symbols: list[str] = parse_functions(tu.cursor, seen_symbols)

    import_result: str = import_template.render(symbols=symbols)

    write_to_file_safely(output_file, import_result)
