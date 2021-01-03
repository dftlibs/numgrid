use reqwest::header::{HeaderMap, HeaderValue, USER_AGENT};
use serde::Deserialize;
use std::collections::HashMap;

#[derive(Deserialize, Debug)]
struct Basis {
    elements: HashMap<String, ElementInfo>,
}

#[derive(Deserialize, Debug)]
struct ElementInfo {
    electron_shells: Vec<Shell>,
}

#[derive(Deserialize, Debug)]
struct Shell {
    angular_momentum: Vec<usize>,
    exponents: Vec<String>,
}

pub fn ang_min_and_max(basis_set: &str, element: usize) -> (HashMap<usize, f64>, f64) {
    let mut headers = HeaderMap::new();
    headers.insert(USER_AGENT, HeaderValue::from_static("reqwest"));

    let url = format!(
        "http://basissetexchange.org/api/basis/{}/format/json?elements={}",
        basis_set.to_string(),
        element.to_string()
    );

    let resp = reqwest::blocking::Client::new()
        .get(&url)
        .headers(headers)
        .send()
        .unwrap()
        .json::<Basis>()
        .unwrap();

    let element = resp.elements.get(&element.to_string()).unwrap();

    let mut alpha_max = -std::f64::MAX;
    let mut alpha_min = HashMap::new();

    for shell in &element.electron_shells {
        let angular_momentum = shell.angular_momentum[0];
        for exponent_string in &shell.exponents {
            let exponent: f64 = exponent_string.parse().unwrap();
            if exponent > alpha_max {
                alpha_max = exponent;
            }
            let s = alpha_min.entry(angular_momentum).or_insert(std::f64::MAX);
            if &exponent < s {
                *s = exponent;
            }
        }
    }

    (alpha_min, alpha_max)
}
