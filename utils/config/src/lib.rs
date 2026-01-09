use std::collections::HashMap;

use serde::{Serialize, Deserialize};
use serde_json::Value;

#[derive(Debug, Serialize, Deserialize)]
struct Settings {

}

#[derive(Serialize, Deserialize)]
struct Module {
    enabled: bool,
    settings: Value,
}

#[derive(Serialize, Deserialize)]
struct Config {
    #[serde(flatten)]
    modules: HashMap<String, Module>,
}

#[unsafe(no_mangle)]
pub extern "C" fn load_config(json: str) {
    let config: Config = serde_json::from_str(&json);

    for (name, module) in &config.modules {
        
    }
}
