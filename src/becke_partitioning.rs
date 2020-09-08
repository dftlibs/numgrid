use crate::bragg;
use crate::parameters;

#[cfg(test)]
use crate::comparison;

// JCP 88, 2547 (1988), eq. 20
#[inline]
fn f3(x: f64, hardness: usize) -> f64 {
    let mut f = x;

    for _ in 0..hardness {
        f *= 1.5 - 0.5 * f * f;
    }

    f
}

#[test]
fn test_f3() {
    assert!(comparison::floats_are_same(
        f3(-0.055317, 3),
        -0.18512279618232905,
        1.0e-15
    ));
    assert!(comparison::floats_are_same(
        f3(-0.491055, 3),
        -0.97223000595777,
        1.0e-15
    ));
    assert!(comparison::floats_are_same(
        f3(-0.974945, 3),
        -0.9999999999974357,
        1.0e-15
    ));
    assert!(comparison::floats_are_same(
        f3(0.644797, 3),
        0.9976980942759357,
        1.0e-15
    ));
    assert!(comparison::floats_are_same(
        f3(0.878986, 3),
        0.9999993429837151,
        1.0e-15
    ));
}

fn distance(p1: &(f64, f64, f64), p2: &(f64, f64, f64)) -> f64 {
    let dx = p1.0 - p2.0;
    let dy = p1.1 - p2.1;
    let dz = p1.2 - p2.2;

    (dx * dx + dy * dy + dz * dz).sqrt()
}

// JCP 88, 2547 (1988)
pub fn partitioning_weight(
    num_centers: usize,
    proton_charges: &Vec<i32>,
    center_coordinates_bohr: &Vec<(f64, f64, f64)>,
    center_index: usize,
    grid_coordinates_bohr: &Vec<(f64, f64, f64)>,
    hardness: usize,
) -> Vec<f64> {
    let mut becke_weights = Vec::new();

    for grid_coordinate in grid_coordinates_bohr {
        let mut pa = vec![1.0; num_centers];

        for ia in 0..num_centers {
            let dist_a = distance(&grid_coordinate, &center_coordinates_bohr[ia]);

            let r_a = bragg::get_bragg_angstrom(proton_charges[ia]);

            for ib in 0..ia {
                let dist_b = distance(&grid_coordinate, &center_coordinates_bohr[ib]);

                let r_b = bragg::get_bragg_angstrom(proton_charges[ib]);

                let dist_ab = distance(&center_coordinates_bohr[ia], &center_coordinates_bohr[ib]);

                // JCP 88, 2547 (1988), eq. 11
                let mu_ab = (dist_a - dist_b) / dist_ab;

                let mut nu_ab = mu_ab;
                if (r_a - r_b).abs() > parameters::SMALL {
                    let u_ab = (r_a + r_b) / (r_b - r_a);
                    let mut a_ab = u_ab / (u_ab * u_ab - 1.0);

                    // JCP 88, 2547 (1988), eq. A3
                    a_ab = a_ab.min(0.5);
                    a_ab = a_ab.max(-0.5);

                    nu_ab += a_ab * (1.0 - mu_ab * mu_ab);
                }

                let f = f3(nu_ab, hardness);

                if (1.0 - f).abs() > parameters::SMALL {
                    pa[ia] *= 0.5 * (1.0 - f);
                    pa[ib] *= 0.5 * (1.0 + f);
                } else {
                    // avoid numerical issues
                    pa[ia] = 0.0;
                }
            }
        }

        let w: f64 = pa.iter().sum();

        if w.abs() > parameters::SMALL {
            becke_weights.push(pa[center_index] / w);
        } else {
            becke_weights.push(1.0);
        }
    }

    return becke_weights;
}
