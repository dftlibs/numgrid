use pyo3::PyErr;
use pyo3::exceptions::{PyRuntimeError, PyValueError};
use std::fmt;

// we deliberately do not use PyErr in the public Rust API: the pyo3
// extension-module feature does not link libpython, so a Rust binary that
// touches PyErr cannot be linked
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum NumgridError {
    // the caller asked for something this library cannot provide
    InvalidInput(String),
    // something outside this library failed, for instance the network
    External(String),
}

impl fmt::Display for NumgridError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::InvalidInput(message) | Self::External(message) => write!(f, "{message}"),
        }
    }
}

impl std::error::Error for NumgridError {}

impl From<NumgridError> for PyErr {
    fn from(error: NumgridError) -> Self {
        match error {
            NumgridError::InvalidInput(message) => PyValueError::new_err(message),
            NumgridError::External(message) => PyRuntimeError::new_err(message),
        }
    }
}
