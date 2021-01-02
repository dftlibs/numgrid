use crate::becke_partitioning;
use crate::bragg;
use crate::bse;
use crate::lebedev;
use crate::radial;

use rayon::prelude::*;

pub fn atom_grid_bse(
    basis_set: &str,
    radial_precision: f64,
    min_num_angular_points: usize,
    max_num_angular_points: usize,
    num_centers: usize,
    proton_charges: Vec<i32>,
    center_index: usize,
    center_coordinates_bohr: Vec<(f64, f64, f64)>,
    hardness: usize,
) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let (alpha_min, alpha_max) =
        bse::ang_min_and_max(basis_set, proton_charges[center_index] as usize);
    let max_l_quantum_number = alpha_min.len() - 1;

    atom_grid(
        radial_precision,
        alpha_min,
        alpha_max,
        max_l_quantum_number,
        min_num_angular_points,
        max_num_angular_points,
        num_centers,
        proton_charges,
        center_index,
        center_coordinates_bohr,
        hardness,
    )
}

pub fn atom_grid(
    radial_precision: f64,
    alpha_min: Vec<f64>,
    alpha_max: f64,
    max_l_quantum_number: usize,
    min_num_angular_points: usize,
    max_num_angular_points: usize,
    num_centers: usize,
    proton_charges: Vec<i32>,
    center_index: usize,
    center_coordinates_bohr: Vec<(f64, f64, f64)>,
    hardness: usize,
) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let (rs, weights_radial) = radial::radial_grid(
        radial_precision,
        alpha_min,
        alpha_max,
        max_l_quantum_number,
        proton_charges[center_index],
    );

    // factors match DIRAC code
    let rb = bragg::get_bragg_angstrom(proton_charges[center_index]) / (5.0 * 0.529177249);

    let mut coordinates = Vec::new();
    let mut weights = Vec::new();

    let pi = std::f64::consts::PI;

    let cx = center_coordinates_bohr[center_index].0;
    let cy = center_coordinates_bohr[center_index].1;
    let cz = center_coordinates_bohr[center_index].2;

    for (&r, &weight_radial) in rs.iter().zip(weights_radial.iter()) {
        // we read the angular grid at each radial step because of pruning
        // this can be optimized
        let mut num_angular = max_num_angular_points;
        if r < rb {
            num_angular = ((max_num_angular_points as f64) * r / rb) as usize;
            num_angular = lebedev::get_closest_num_angular(num_angular);
            if num_angular < min_num_angular_points {
                num_angular = min_num_angular_points;
            }
        }
        let (coordinates_angular, weights_angular) = lebedev::angular_grid(num_angular);

        let wt = 4.0 * pi * weight_radial;
        for (&xyz, &weight_angular) in coordinates_angular.iter().zip(weights_angular.iter()) {
            let x = cx + r * xyz.0;
            let y = cy + r * xyz.1;
            let z = cz + r * xyz.2;

            coordinates.push((x, y, z));
            weights.push(wt * weight_angular);
        }
    }

    if num_centers > 1 {
        let w_partitioning: Vec<f64> = coordinates
            .par_iter()
            .map(|c| {
                becke_partitioning::partitioning_weight(
                    center_index,
                    &center_coordinates_bohr,
                    &proton_charges,
                    *c,
                    hardness,
                )
            })
            .collect();

        for (i, w) in weights.iter_mut().enumerate() {
            *w *= w_partitioning[i];
        }
    }

    (coordinates, weights)
}
