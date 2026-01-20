use std::{env, fs, path::Path};

fn embed_file(path: &Path, base_name: &str, out: &mut String) {
    out.push_str(&format!(
        r#"
    EmbeddedAsset {{
        name: b"{name}",
        data: include_bytes!(r"{path}"),
    }},
        "#,
        name = base_name,
        path = path.display(),
    ));
}

fn visit_dir(dir: &Path, base: &Path, out: &mut String) {
    for entry in fs::read_dir(dir).unwrap() {
        let entry = entry.unwrap();
        let path = entry.path();

        if path.is_dir() {
            visit_dir(&path, base, out);
        } else {
            let rel = path.strip_prefix(base).unwrap();
            let name = rel.to_string_lossy().replace("\\", "/");

            out.push_str(&format!(
                r#"
    EmbeddedAsset {{
        name: b"{name}",
        data: include_bytes!(r"{path}"),
    }},
                "#,
                name = name,
                path = path.display(),
            ));
        }
    }
}

fn main() {
    let assets_dir_env = env::var("RUST_ASSETS_DIR").expect("RUST_ASSETS_DIR is not set");
    let signatures_file = env::var("RUST_SIGNATURES_FILE").expect("RUST_SIGNATURES_FILE is not set");

    println!("cargo:rerun-if-changed=RUST_SIGNATURES_FILE");
    println!("cargo:rerun-if-changed=RUST_ASSETS_DIR");

    let assets_dir = Path::new(&assets_dir_env);
    let signatures_file = Path::new(&signatures_file);

    let mut out = String::new();

    visit_dir(assets_dir, assets_dir, &mut out);

    if signatures_file.exists() {
        embed_file(signatures_file, "signatures.json", &mut out);
    }

    let generated = format!(
        r#"
struct EmbeddedAsset {{
    name: &'static [u8],
    data: &'static [u8],
}}

static ASSETS: &[EmbeddedAsset] = &[
{assets}
];
        "#,
        assets = out,
    );

    let out_dir = std::env::var("OUT_DIR").unwrap();
    let dest = Path::new(&out_dir).join("assets.rs");
    fs::write(dest, generated).unwrap();
}