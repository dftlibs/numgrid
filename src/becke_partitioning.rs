use crate::Result;
use crate::bragg;
use crate::parameters;

#[cfg(test)]
use crate::comparison;

// JCP 88, 2547 (1988), eq. 20
#[inline]
fn f3(x: f64, hardness: usize) -> f64 {
    (0..hardness).fold(x, |f, _| f * (1.5 - 0.5 * f * f))
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
//
// this runs once per grid point, so the caller hands in the per-center scratch
// buffer instead of us allocating a fresh one every time. it must hold one
// entry per center and is reset here, so its incoming contents do not matter
pub(crate) fn partitioning_weight(
    center_index: usize,
    center_coordinates_bohr: &[(f64, f64, f64)],
    proton_charges: &[i32],
    grid_coordinates_bohr: (f64, f64, f64),
    hardness: usize,
    pa: &mut [f64],
) -> Result<f64> {
    let num_centers = proton_charges.len();

    pa.fill(1.0);

    for ia in 0..num_centers {
        let dist_a = distance(&grid_coordinates_bohr, &center_coordinates_bohr[ia]);

        let r_a = bragg::get_bragg_angstrom(proton_charges[ia])?;

        for ib in 0..ia {
            let dist_b = distance(&grid_coordinates_bohr, &center_coordinates_bohr[ib]);

            let r_b = bragg::get_bragg_angstrom(proton_charges[ib])?;

            let dist_ab = distance(&center_coordinates_bohr[ia], &center_coordinates_bohr[ib]);

            // JCP 88, 2547 (1988), eq. 11
            let mu_ab = (dist_a - dist_b) / dist_ab;

            let nu_ab = if (r_a - r_b).abs() > parameters::SMALL {
                let u_ab = (r_a + r_b) / (r_b - r_a);
                let a_ab = u_ab / (u_ab * u_ab - 1.0);

                mu_ab + a_ab.clamp(-0.5, 0.5) * (1.0 - mu_ab * mu_ab)
            } else {
                mu_ab
            };

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
        Ok(pa[center_index] / w)
    } else {
        Ok(1.0)
    }
}
