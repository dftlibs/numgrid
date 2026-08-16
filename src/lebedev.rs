use pyo3::prelude::*;

use crate::tables;
use crate::{Grid, NumgridError, Result};

#[pyfunction]
pub fn angular_grid(num_points: usize) -> Result<Grid> {
    let offsets = tables::offsets::offsets();

    let offset: usize = *offsets.get(&num_points).ok_or_else(|| {
        NumgridError::InvalidInput(format!(
            "angular_grid called with unsupported num_points {}, allowed are: {:?}",
            num_points,
            valid_input_values()
        ))
    })?;

    Ok((
        tables::coordinates::COORDINATES[offset..(offset + num_points)].to_vec(),
        tables::weights::WEIGHTS[offset..(offset + num_points)].to_vec(),
    ))
}

pub fn get_closest_num_angular(n: usize) -> Result<usize> {
    valid_input_values()
        .into_iter()
        .find(|&number| number >= n)
        .ok_or_else(|| {
            NumgridError::InvalidInput(format!(
                "input n too high in get_closest_num_angular: {}, largest supported is {:?}",
                n,
                valid_input_values().last()
            ))
        })
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
