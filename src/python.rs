use pyo3::prelude::*;
use pyo3::wrap_pyfunction;

use crate::atom::__pyo3_get_function_atom_grid;
use crate::atom::__pyo3_get_function_atom_grid_bse;
use crate::lebedev::__pyo3_get_function_angular_grid;
use crate::radial::__pyo3_get_function_radial_grid_kk;
use crate::radial::__pyo3_get_function_radial_grid_lmg;

#[pymodule]
fn numgrid(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add("__version__", env!("CARGO_PKG_VERSION"))?;

    m.add_function(wrap_pyfunction!(atom_grid, m)?)?;
    m.add_function(wrap_pyfunction!(atom_grid_bse, m)?)?;
    m.add_function(wrap_pyfunction!(angular_grid, m)?)?;
    m.add_function(wrap_pyfunction!(radial_grid_lmg, m)?)?;
    m.add_function(wrap_pyfunction!(radial_grid_kk, m)?)?;

    Ok(())
}
