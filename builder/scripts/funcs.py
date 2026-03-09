"""
This script defines all the virtual functions
from index signatures in the actual c++ code
"""
import json
import ctypes
from ctypes import wintypes
from pathlib import Path
from collections import defaultdict


dbghelp = ctypes.WinDLL("Dbghelp.dll")

UnDecorateSymbolName = dbghelp.UnDecorateSymbolName
UnDecorateSymbolName.argtypes = [
    wintypes.LPCSTR,
    wintypes.LPSTR,
    wintypes.DWORD,
]
UnDecorateSymbolName.restype = wintypes.DWORD

UNDNAME_COMPLETE = 0x0000


def make_fake_import_lib(symbols: list[tuple[str, str]], output_path: Path) -> None:
    lines = []
    lines.append("LIBRARY mcapi.dll")
    lines.append("EXPORTS")

    for symbol in symbols:
        lines.append(f"    ; {symbol[0]}")
        lines.append(f"    {symbol[1]}\n")
    
    lib_text = "\n".join(lines)

    original_text = output_path.read_text()

    if original_text != lib_text:
        output_path.write_text(lib_text)


def undecorate_symbol(symbol: str):
    buffer = ctypes.create_string_buffer(2048)

    UnDecorateSymbolName(
        symbol.encode("ascii"),
        buffer,
        len(buffer),
        UNDNAME_COMPLETE,
    )

    return buffer.value.decode("ascii")


def generate_funcs(current_source_dir: Path, current_binary_dir: Path) -> None:
    signatures_path = current_source_dir / "signatures.embed.json"
    output_path = current_binary_dir / "import.def"

    with open(signatures_path, "r") as file:
        data = json.load(file)

    classes = defaultdict(list)
    symbol_pairs = []

    for key, entry in data.items():
        if "$$" in key:
            class_name, function_name = key.split("$$", 1)
            classes[class_name].append(function_name)
        else:
            symbol = entry.get("symbol")
            if symbol:
                symbol_pairs.append((key, symbol))
    
    make_fake_import_lib(symbol_pairs, output_path)
