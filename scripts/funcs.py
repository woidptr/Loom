"""
This script defines all the virtual functions
from index signatures in the actual c++ code
"""
import sys
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


def fill_in_virtual_funcs(vfuncs: dict) -> None:
    pass


def main() -> None:
    if len(sys.argv) < 2:
        print("Error: No signatures file provided")
        sys.exit(1)
    
    signatures_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])

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
    
    # classes = dict(classes)

    # print(classes)

    # sdk_dir = Path("../src/sdk")

    # for path in sdk_dir.rglob("*.cpp"):
    #     class_name = path.stem

    #     if class_name in classes:
    #         functions = classes[class_name]

    #         with open(path, "a", encoding="utf-8") as f:
    #             for func in functions:
    #                 f.write(f"\nvoid {class_name}::{func}() {{\n")
    #                 f.write(f'    return Memory::CallVirtual<decltype(&{class_name}::{func})>($get_index("{class_name}$${func}"), *this);\n')
    #                 f.write("}\n")

    # for class_name in classes:
    #     target_file = f"{class_name}.cpp"

    #     for path in sdk_dir.rglob("*.cpp"):
    #         if path.name == target_file:
    #             print(f"Found: {path}")


if __name__ == "__main__":
    main()
