use pyo3::prelude::*;

use crate::atom::atom_grid;
use crate::atom::atom_grid_bse;
use crate::lebedev::angular_grid;
use crate::radial::radial_grid_kk;
use crate::radial::radial_grid_lmg;
use crate::radial::radial_grid_lmg_bse;

#[pymodule]
fn numgrid(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add("__version__", env!("CARGO_PKG_VERSION"))?;

    m.add_function(wrap_pyfunction!(atom_grid, m)?)?;
    m.add_function(wrap_pyfunction!(atom_grid_bse, m)?)?;
    m.add_function(wrap_pyfunction!(angular_grid, m)?)?;
    m.add_function(wrap_pyfunction!(radial_grid_kk, m)?)?;
    m.add_function(wrap_pyfunction!(radial_grid_lmg, m)?)?;
    m.add_function(wrap_pyfunction!(radial_grid_lmg_bse, m)?)?;

    Ok(())
}
