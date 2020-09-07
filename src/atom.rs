use crate::becke_partitioning;
use crate::lebedev;
use crate::radial;

pub fn atom_grid(
    radial_precision: f64,
    alpha_min: &Vec<f64>,
    alpha_max: f64,
    max_l_quantum_number: usize,
    num_angular_points: usize,
    num_centers: usize,
    proton_charges: &Vec<i32>,
    center_index: usize,
    x_coordinates_bohr: &Vec<f64>,
    y_coordinates_bohr: &Vec<f64>,
    z_coordinates_bohr: &Vec<f64>,
    becke_hardness: usize,
) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let (rs, weights_radial) = radial::radial_grid(
        radial_precision,
        &alpha_min,
        alpha_max,
        max_l_quantum_number,
        proton_charges[center_index],
    );
    let (coordinates_angular, weights_angular) = lebedev::angular_grid(num_angular_points);

    let mut coordinates = Vec::new();
    let mut weights = Vec::new();

    let pi = std::f64::consts::PI;

    let cx = x_coordinates_bohr[center_index];
    let cy = y_coordinates_bohr[center_index];
    let cz = z_coordinates_bohr[center_index];

    for (&r, &weight_radial) in rs.iter().zip(weights_radial.iter()) {
        let wt = 4.0 * pi * weight_radial;
        for (&xyz, &weight_angular) in coordinates_angular.iter().zip(weights_angular.iter()) {
            let x = cx + r * xyz.0;
            let y = cy + r * xyz.1;
            let z = cz + r * xyz.2;

            let w_becke = if num_centers > 1 {
                becke_partitioning::partitioning_weight(
                    num_centers,
                    &proton_charges,
                    &x_coordinates_bohr,
                    &y_coordinates_bohr,
                    &z_coordinates_bohr,
                    center_index,
                    x,
                    y,
                    z,
                    becke_hardness,
                )
            } else {
                1.0
            };

            coordinates.push((x, y, z));
            weights.push(wt * weight_angular * w_becke);
        }
    }

    (coordinates, weights)
}
