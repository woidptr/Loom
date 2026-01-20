use core::ffi::{c_char, CStr};
use core::ptr;
use embedder::get;

#[repr(C)]
pub struct RawAsset {
    pub data: *const u8,
    pub size: usize,
}

#[unsafe(no_mangle)]
pub extern "C" fn get_asset(name: *const c_char) -> RawAsset {
    if name.is_null() {
        return RawAsset { data: ptr::null(), size: 0 };
    }

    let c_str = unsafe { CStr::from_ptr(name) };

    let r_str = match c_str.to_str() {
        Ok(s) => s,
        Err(_) => return RawAsset { data: ptr::null(), size: 0 },
    };

    match embedder::get(r_str) {
        Some(data) => RawAsset {
            data: data.as_ptr(),
            size: data.len(),
        },
        None => RawAsset {
            data: ptr::null(),
            size: 0,
        },
    }
}