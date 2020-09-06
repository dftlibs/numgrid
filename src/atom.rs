use crate::lebedev;
use crate::radial;

pub fn get_atom_grid(
    radial_precision: f64,
    alpha_min: Vec<f64>,
    alpha_max: f64,
    max_l_quantum_number: usize,
    proton_charge: i32,
    num_angular_points: usize,
) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let (rs, weights_radial) = radial::get_radial_grid(
        radial_precision,
        alpha_min,
        alpha_max,
        max_l_quantum_number,
        proton_charge,
    );
    let (coordinates_angular, weights_angular) = lebedev::get_angular_grid(num_angular_points);

    let mut coordinates = Vec::new();
    let mut weights = Vec::new();

    let pi = std::f64::consts::PI;

    for (&r, &weight_radial) in rs.iter().zip(weights_radial.iter()) {
        let wt = 4.0 * pi * weight_radial;
        for (&xyz, &weight_angular) in coordinates_angular.iter().zip(weights_angular.iter()) {
            coordinates.push((xyz.0 * r, xyz.1 * r, xyz.2 * r));
            weights.push(wt * weight_angular);
        }
    }

    (coordinates, weights)
}
