import sys
from pathlib import Path
from builder.scripts.embedder import embed
from builder.scripts.funcs import generate_funcs


def main() -> None:
    current_source_dir = Path(sys.argv[1])
    current_binary_dir = Path(sys.argv[2])

    embed(current_source_dir, current_binary_dir)
    generate_funcs(current_source_dir, current_binary_dir)


if __name__ == "__main__":
    main()
