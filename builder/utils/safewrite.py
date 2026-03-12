from pathlib import Path


def write_to_file_safely(path: Path, content: str):
    if not path.is_file():
        return

    original_content: str

    if path.exists():
        original_content = path.read_text()
    else:
        original_content = ""
    
    if original_content != content:
        path.write_text(content)
