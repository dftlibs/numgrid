use pyo3::prelude::*;
use std::collections::HashMap;
use std::sync::LazyLock;

use crate::tables;
use crate::{Grid, GridSlices, NumgridError, Result};

// the offset table and the sorted list of supported point counts are read on
// every radial step, so we build them once instead of on each call
static OFFSETS: LazyLock<HashMap<usize, usize>> = LazyLock::new(tables::offsets::offsets);

static VALID_INPUT_VALUES: LazyLock<Vec<usize>> = LazyLock::new(|| {
    let mut valid_numbers: Vec<usize> = OFFSETS.keys().copied().collect();
    valid_numbers.sort_unstable();

    valid_numbers
});

// borrows into the static tables, for callers that only read the grid
pub(crate) fn angular_grid_slices(num_points: usize) -> Result<GridSlices> {
    let offset = *OFFSETS.get(&num_points).ok_or_else(|| {
        NumgridError::InvalidInput(format!(
            "angular_grid called with unsupported num_points {}, allowed are: {:?}",
            num_points, *VALID_INPUT_VALUES
        ))
    })?;

    let range = offset..(offset + num_points);

    Ok((
        &tables::coordinates::COORDINATES[range.clone()],
        &tables::weights::WEIGHTS[range],
    ))
}

#[pyfunction]
pub fn angular_grid(num_points: usize) -> Result<Grid> {
    let (coordinates, weights) = angular_grid_slices(num_points)?;

    Ok((coordinates.to_vec(), weights.to_vec()))
}

pub(crate) fn get_closest_num_angular(n: usize) -> Result<usize> {
    VALID_INPUT_VALUES
        .iter()
        .copied()
        .find(|&number| number >= n)
        .ok_or_else(|| {
            NumgridError::InvalidInput(format!(
                "input n too high in get_closest_num_angular: {}, largest supported is {:?}",
                n,
                VALID_INPUT_VALUES.last()
            ))
        })
}
