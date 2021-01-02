#![allow(dead_code)]

pub fn floats_are_same(value: f64, reference: f64, threshold: f64) -> bool {
    let absolute_error = (value - reference).abs();
    if reference.abs() > threshold {
        let relative_error = (absolute_error / reference).abs();
        relative_error < threshold
    } else {
        absolute_error < threshold
    }
}
