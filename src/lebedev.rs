use pyo3::prelude::*;

use crate::tables;

#[pyfunction]
pub fn angular_grid(num_points: usize) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let offsets = tables::offsets::offsets();

    let offset: usize = match offsets.get(&num_points) {
        Some(v) => *v,
        None => panic!(
            "angular_grid called with unsupported num_points, allowed are: {:?}",
            valid_input_values()
        ),
    };

    (
        tables::coordinates::COORDINATES[offset..(offset + num_points)].to_vec(),
        tables::weights::WEIGHTS[offset..(offset + num_points)].to_vec(),
    )
}

pub fn get_closest_num_angular(n: usize) -> usize {
    for number in valid_input_values() {
        if number >= n {
            return number;
        }
    }
    panic!("input n too high in get_closest_num_angular");
}

fn valid_input_values() -> Vec<usize> {
    let offsets = tables::offsets::offsets();

    let mut valid_numbers = Vec::new();
    for (k, _) in offsets {
        valid_numbers.push(k);
    }
    valid_numbers.sort_unstable();

    valid_numbers
}
