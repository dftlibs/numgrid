//! Describe me ...

mod atom;
mod becke_partitioning;
mod bragg;
mod bse;
#[cfg(test)]
mod comparison;
mod error;
mod lebedev;
mod parameters;
mod python;
mod radial;
mod tables;

pub use crate::error::NumgridError;

/// grid point coordinates in bohr paired with their integration weights
pub type Grid = (Vec<(f64, f64, f64)>, Vec<f64>);

/// radial grid points in bohr paired with their integration weights
pub type RadialGrid = (Vec<f64>, Vec<f64>);

/// borrowed view into the static angular grid tables
pub(crate) type GridSlices = (&'static [(f64, f64, f64)], &'static [f64]);

/// result type of the fallible entry points of this crate
pub type Result<T> = std::result::Result<T, NumgridError>;

pub use crate::atom::atom_grid;
pub use crate::atom::atom_grid_bse;
pub use crate::lebedev::angular_grid;
pub use crate::radial::radial_grid_kk;
pub use crate::radial::radial_grid_lmg;
pub use crate::radial::radial_grid_lmg_bse;
