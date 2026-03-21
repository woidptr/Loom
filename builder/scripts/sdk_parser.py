import re
import json
import shlex
from pathlib import Path
from dataclasses import dataclass
from jinja2 import Template
from clang.cindex import Index, CursorKind, Cursor, TranslationUnit

from builder.utils.enums import SignatureType
from builder.utils.context import ClangParserContext, ExtractedSignature
from builder.utils.safewrite import write_to_file_safely


@dataclass(frozen=True)
class ExtractedSymbol:
    qualified_name: str
    mangled_name: str


def generate_umbrella_code(sdk_path: Path) -> str:
    includes = []
    for path in sdk_path.rglob("*.hpp"):
        safe_path = path.resolve().as_posix()
        includes.append(f'#include "{safe_path}"')
    
    umbrella_code: str = "\n".join(includes)

    return umbrella_code

def get_fully_qualified_name(cursor: Cursor) -> str:
    name: str = cursor.spelling
    parent: Cursor | None = cursor.semantic_parent

    while parent and parent.kind != CursorKind.TRANSLATION_UNIT:
        if parent.spelling:
            name = f"{parent.spelling}::{name}"
        parent = parent.semantic_parent
    
    return name


def parse_ast(cursor: Cursor, seen_symbols: set, sdk_dir_str: str) -> tuple[list[ExtractedSymbol], list[ExtractedSignature]]:
    found_symbols: list[ExtractedSymbol] = []
    found_signatures: list[ExtractedSignature] = []

    if cursor.location.file:
        file_path = str(cursor.location.file.name).replace("\\", "/").lower()

        if sdk_dir_str not in file_path and "umbrella.cpp" not in file_path:
            return [], []

    # Extract metadata from comments 
    if cursor.raw_comment:
        sig_match = re.search(r"@sig\s*\{([^}]+)\}", cursor.raw_comment)

        if sig_match:
            sig: str = sig_match.group(1).strip()

            type_match = re.search(r"@type\s*([a-zA-Z_]+)", cursor.raw_comment)
            type: str = type_match.group(1).strip().lower() if type_match else "direct"

            name_match = re.search(r"@name\s*([\w:]+)", cursor.raw_comment)
            custom_name: str | None = name_match.group(1).strip() if name_match else None

            sig_type: SignatureType = SignatureType.DIRECT
            if type == "field":
                sig_type = SignatureType.FIELD
            elif type == "vtable_index":
                sig_type = SignatureType.VTABLE_INDEX
            elif type == "vtable_ptr":
                sig_type = SignatureType.VTABLE_PTR
            
            final_name: str = custom_name if custom_name else get_fully_qualified_name(cursor)

            found_signatures.append(ExtractedSignature(
                name=final_name,
                pattern=sig,
                type=sig_type,
            ))

    # Extract dll imports
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

                found_symbols.append(ExtractedSymbol(
                    qualified_name=qualified_name,
                    mangled_name=mangled,
                ))
    
    # Recurse into children
    for child_node in cursor.get_children():
        child_syms, child_sigs = parse_ast(child_node, seen_symbols, sdk_dir_str)
        found_symbols.extend(child_syms)
        found_signatures.extend(child_sigs)
    
    return found_symbols, found_signatures


def extract_cmake_flags(compdb_path: Path) -> list[str]:
    with open(compdb_path, "r", encoding="utf-8") as f:
        db = json.load(f)
    
    command = db[0].get("command", "")

    args = shlex.split(command, posix=False)

    clang_args: list[str] = []

    for arg in args:
        arg = arg.strip('"').strip("'")

        if arg.startswith("-I"):
            clang_args.append(arg)
        elif arg.startswith("-imsvc"):
            path = arg[6:]
            clang_args.append(f"-isystem{path}")
        elif arg.startswith("/I"):
            path = arg[2:]
            clang_args.append(f"-I{path}")
        elif arg.startswith("-D"):
            if '"' in arg or "'" in arg or "\\" in arg:
                continue
            
            if "SPDLOG_COMPILED_LIB" in arg or "ZYDIS_STATIC_BUILD" in arg:
                pass

            clang_args.append(arg)
    
    return clang_args


def parse_cpp_sdk(ctx: ClangParserContext) -> tuple[list[ExtractedSymbol], list[ExtractedSignature]]:
    index: Index = Index.create()
    seen_symbols: set[str] = set()

    umbrella_code: str = generate_umbrella_code(ctx.sdk_path)
    cmake_args: list[str] = extract_cmake_flags(ctx.compile_commands_path)

    clang_args = [
        "-x", "c++", 
        "-std=c++23", 
        "-fms-extensions",
        f"-I{ctx.source_dir / 'src'}",
        "-D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH",
        "-Wno-pragma-once-outside-header"
    ] + cmake_args

    tu: TranslationUnit = index.parse(
        "umbrella.cpp",
        args=clang_args,
        unsaved_files=[("umbrella.cpp", umbrella_code)],
    )

    for diag in tu.diagnostics:
        severity_names = ['Ignored', 'Note', 'Warning', 'Error', 'Fatal']
        severity = severity_names[diag.severity] if diag.severity < len(severity_names) else 'Unknown'

        if diag.location.file:
            file_name = diag.location.file.name
            line = diag.location.line
            col = diag.location.column
            print(f"  [{severity}] {file_name}:{line}:{col} - {diag.spelling}")
        else:
            print(f"  [{severity}] Unknown Location - {diag.spelling}")

    import_symbols, signature_list = parse_ast(tu.cursor, seen_symbols, ctx.sdk_path.as_posix().lower())

    return import_symbols, signature_list


def generate_fake_imports(ctx: ClangParserContext, symbols: list[ExtractedSymbol]) -> None:
    import_template_text: str = ctx.template_import_path.read_text()
    import_template: Template = Template(import_template_text)

    import_result: str = import_template.render(symbols=symbols)
    write_to_file_safely(ctx.result_import_path, import_result)


def generate_signatures(ctx: ClangParserContext, signatures: list[ExtractedSignature]) -> None:
    sig_template_text: str = ctx.template_signatures_path.read_text()
    sig_template: Template = Template(sig_template_text)

    sig_result: str = sig_template.render(signatures=signatures)
    write_to_file_safely(ctx.result_signatures_path, sig_result)


def generate_sdk_metadata(ctx: ClangParserContext) -> None:
    dll_imports, signatures = parse_cpp_sdk(ctx)

    generate_fake_imports(ctx, dll_imports)
    generate_signatures(ctx, signatures)
