use crate::{NumgridError, Result};
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

// reqwest errors render as "error sending request for url (...)" and keep the
// interesting part in the source chain, so we spell the whole chain out
fn error_chain(error: &dyn std::error::Error) -> String {
    std::iter::successors(Some(error), |e| e.source())
        .map(ToString::to_string)
        .collect::<Vec<_>>()
        .join(": ")
}

pub(crate) fn element_from_proton_charge(proton_charge: i32) -> Result<usize> {
    usize::try_from(proton_charge).map_err(|e| {
        NumgridError::InvalidInput(format!(
            "proton charge {proton_charge} is not a valid element: {e}"
        ))
    })
}

pub(crate) fn ang_min_and_max(
    basis_set: &str,
    element: usize,
) -> Result<(HashMap<usize, f64>, f64)> {
    let mut headers = HeaderMap::new();
    headers.insert(USER_AGENT, HeaderValue::from_static("reqwest"));

    let url = format!(
        "https://basissetexchange.org/api/basis/{}/format/json?elements={}",
        basis_set, element,
    );

    let resp = reqwest::blocking::Client::new()
        .get(url)
        .headers(headers)
        .send()
        .map_err(|e| {
            NumgridError::External(format!(
                "could not query the basis set exchange: {}",
                error_chain(&e)
            ))
        })?
        .json::<Basis>()
        .map_err(|e| {
            NumgridError::External(format!(
                "could not parse the response from the basis set exchange: {}",
                error_chain(&e)
            ))
        })?;

    let element_info = resp.elements.get(&element.to_string()).ok_or_else(|| {
        NumgridError::InvalidInput(format!(
            "basis set {basis_set} does not contain element {element}"
        ))
    })?;

    let mut alpha_max = -f64::MAX;
    let mut alpha_min = HashMap::new();

    for shell in &element_info.electron_shells {
        let angular_momentum = shell.angular_momentum[0];
        for exponent_string in &shell.exponents {
            let exponent: f64 = exponent_string.parse().map_err(|e| {
                NumgridError::InvalidInput(format!(
                    "could not parse exponent {exponent_string}: {e}"
                ))
            })?;
            alpha_max = alpha_max.max(exponent);
            let s = alpha_min.entry(angular_momentum).or_insert(f64::MAX);
            if &exponent < s {
                *s = exponent;
            }
        }
    }

    Ok((alpha_min, alpha_max))
}
