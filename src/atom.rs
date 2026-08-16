#![expect(
    clippy::too_many_arguments,
    reason = "these are the parameters the Python API exposes; grouping them into a struct would complicate the binding"
)]

use pyo3::prelude::*;
use rayon::prelude::*;

use std::collections::HashMap;

use crate::becke_partitioning;
use crate::bragg;
use crate::bse;
use crate::lebedev;
use crate::parameters;
use crate::radial;
use crate::{Grid, Result};

#[pyfunction]
pub fn atom_grid_bse(
    basis_set: &str,
    radial_precision: f64,
    min_num_angular_points: usize,
    max_num_angular_points: usize,
    proton_charges: Vec<i32>,
    center_index: usize,
    center_coordinates_bohr: Vec<(f64, f64, f64)>,
    hardness: usize,
) -> Result<Grid> {
    let element = bse::element_from_proton_charge(proton_charges[center_index])?;
    let (alpha_min, alpha_max) = bse::ang_min_and_max(basis_set, element)?;

    atom_grid(
        alpha_min,
        alpha_max,
        radial_precision,
        min_num_angular_points,
        max_num_angular_points,
        proton_charges,
        center_index,
        center_coordinates_bohr,
        hardness,
    )
}

#[pyfunction]
pub fn atom_grid(
    alpha_min: HashMap<usize, f64>,
    alpha_max: f64,
    radial_precision: f64,
    min_num_angular_points: usize,
    max_num_angular_points: usize,
    proton_charges: Vec<i32>,
    center_index: usize,
    center_coordinates_bohr: Vec<(f64, f64, f64)>,
    hardness: usize,
) -> Result<Grid> {
    let (rs, weights_radial) = radial::radial_grid_lmg(
        alpha_min,
        alpha_max,
        radial_precision,
        proton_charges[center_index],
    )?;

    // factors match DIRAC code
    let rb = bragg::get_bragg_angstrom(proton_charges[center_index])?
        / (5.0 * parameters::BOHR_IN_ANGSTROM);

    let mut coordinates = Vec::new();
    let mut weights = Vec::new();

    let pi = std::f64::consts::PI;

    let cx = center_coordinates_bohr[center_index].0;
    let cy = center_coordinates_bohr[center_index].1;
    let cz = center_coordinates_bohr[center_index].2;

    for (&r, &weight_radial) in rs.iter().zip(weights_radial.iter()) {
        // we read the angular grid at each radial step because of pruning
        // this can be optimized
        let num_angular = if r < rb {
            #[expect(
                clippy::cast_sign_loss,
                reason = "r and rb are both positive so the product is non-negative"
            )]
            let pruned = ((max_num_angular_points as f64) * r / rb) as usize;

            lebedev::get_closest_num_angular(pruned)?.max(min_num_angular_points)
        } else {
            max_num_angular_points
        };
        let (coordinates_angular, weights_angular) = lebedev::angular_grid_slices(num_angular)?;

        let wt = 4.0 * pi * weight_radial;
        for (&xyz, &weight_angular) in coordinates_angular.iter().zip(weights_angular.iter()) {
            let x = cx + r * xyz.0;
            let y = cy + r * xyz.1;
            let z = cz + r * xyz.2;

            coordinates.push((x, y, z));
            weights.push(wt * weight_angular);
        }
    }

    if center_coordinates_bohr.len() > 1 {
        // map_init hands each rayon thread one scratch buffer to reuse across
        // all the grid points it handles, instead of allocating per point
        let w_partitioning: Vec<f64> = coordinates
            .par_iter()
            .map_init(
                || vec![1.0; proton_charges.len()],
                |pa, c| {
                    becke_partitioning::partitioning_weight(
                        center_index,
                        &center_coordinates_bohr,
                        &proton_charges,
                        *c,
                        hardness,
                        pa,
                    )
                },
            )
            .collect::<Result<Vec<f64>>>()?;

        for (w, wp) in weights.iter_mut().zip(&w_partitioning) {
            *w *= wp;
        }
    }

    Ok((coordinates, weights))
}
