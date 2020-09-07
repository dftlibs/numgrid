use crate::bragg;
use crate::parameters;

#[cfg(test)]
use crate::comparison;

// JCP 88, 2547 (1988), eq. 20
#[inline]
fn f3(x: f64, becke_hardness: usize) -> f64 {
    let mut f = x;

    for _ in 0..becke_hardness {
        f *= 1.5 - 0.5 * f * f;
    }

    f
}

#[test]
fn test_f3() {
    assert!(comparison::floats_are_same(
        f3(-0.055317, 3),
        -0.18512279618232905
    ));
    assert!(comparison::floats_are_same(
        f3(-0.491055, 3),
        -0.97223000595777
    ));
    assert!(comparison::floats_are_same(
        f3(-0.974945, 3),
        -0.9999999999974357
    ));
    assert!(comparison::floats_are_same(
        f3(0.644797, 3),
        0.9976980942759357
    ));
    assert!(comparison::floats_are_same(
        f3(0.878986, 3),
        0.9999993429837151
    ));
}

// JCP 88, 2547 (1988)
pub fn partitioning_weight(
    num_centers: usize,
    proton_charges: &Vec<i32>,
    x_coordinates_bohr: &Vec<f64>,
    y_coordinates_bohr: &Vec<f64>,
    z_coordinates_bohr: &Vec<f64>,
    center_index: usize,
    x: f64,
    y: f64,
    z: f64,
    becke_hardness: usize,
) -> f64 {
    let mut pa = vec![1.0; num_centers];

    for ia in 0..num_centers {
        let vx = x_coordinates_bohr[ia] - x;
        let vy = y_coordinates_bohr[ia] - y;
        let vz = z_coordinates_bohr[ia] - z;
        let dist_a = (vx * vx + vy * vy + vz * vz).sqrt();

        // in principle good idea but fails for larger molecules containing
        // diffuse sets
        // if (ia != center_index) && (dist_a > BECKE_CUTOFF) {
        //     pa[ia] = 0.0;
        //     continue;
        // }

        let r_a = bragg::get_bragg_angstrom(proton_charges[ia]);

        for ib in 0..ia {
            let vx = x_coordinates_bohr[ib] - x;
            let vy = y_coordinates_bohr[ib] - y;
            let vz = z_coordinates_bohr[ib] - z;
            let dist_b = (vx * vx + vy * vy + vz * vz).sqrt();

            let r_b = bragg::get_bragg_angstrom(proton_charges[ib]);

            let vx = x_coordinates_bohr[ib] - x_coordinates_bohr[ia];
            let vy = y_coordinates_bohr[ib] - y_coordinates_bohr[ia];
            let vz = z_coordinates_bohr[ib] - z_coordinates_bohr[ia];
            let dist_ab = (vx * vx + vy * vy + vz * vz).sqrt();

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

            let f = f3(nu_ab, becke_hardness);

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
        return pa[center_index] / w;
    } else {
        return 1.0;
    }
}
