#![no_std]

struct AssetEntry {
    name: &'static [u8],
    data: &'static [u8],
}

include!(concat!(env!("OUT_DIR"), "/assets.rs"));

pub fn get(name: &str) -> Option<&'static [u8]> {
    let name_bytes = name.as_bytes();

    for asset in ASSETS {
        if asset.name == name_bytes {
            return Some(asset.data);
        }
    }

    None
}