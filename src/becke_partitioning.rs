// JCP 88, 2547 (1988), eq. 20
fn g(x: f64) -> f64 {
    let mut f = x;

    for _ in 0..3 {
        f *= 1.5 - 0.5 * f * f;
    }

    f
}

// CPL 257 (1996), page 215, eq. 14
fn z(mu: f64) -> f64 {
    let a = 0.64;
    let t = mu / a;

    (35.0 * t - 35.0 * t.powi(3) + 21.0 * t.powi(5) - 5.0 * t.powi(7)) / 16.0
}

fn distance(p1: &(f64, f64, f64), p2: &(f64, f64, f64)) -> f64 {
    let dx = p1.0 - p2.0;
    let dy = p1.1 - p2.1;
    let dz = p1.2 - p2.2;

    (dx * dx + dy * dy + dz * dz).sqrt()
}

fn mu(r_i: f64, r_j: f64, r_ij: f64) -> f64 {
    (r_i - r_j) / r_ij
}

// JCP 149, 204111 (2018), page 4
pub fn partitioning_weights_lko(
    center_index: usize,
    center_coordinates_bohr: &[(f64, f64, f64)],
    grid_coordinates_bohr: &[(f64, f64, f64)],
    r_cutoff_bohr: f64,
) -> Vec<f64> {
    let num_centers = center_coordinates_bohr.len();

    let mut r_pi = Vec::new();
    for (i, center_coordinate) in center_coordinates_bohr.iter().enumerate() {
        r_pi.push((
            distance(&center_coordinate, &center_coordinates_bohr[center_index]),
            i,
        ));
    }
    r_pi.sort_by(|a, b| a.partial_cmp(b).unwrap());

    let weights: Vec<f64> = grid_coordinates_bohr
        .iter()
        .map(|grid_point| {
            let r_gp = distance(&grid_point, &center_coordinates_bohr[center_index]);

            // CPL 257 (1996), page 216, eq. 15
            if r_gp < 0.5 * (1.0 - 0.64) * r_pi[1].0 {
                return 1.0;
            }

            let mut r_gn = r_gp;
            let mut r_gi = Vec::new();

            // skipping 1 because the first is the same center
            for &(r, i) in r_pi.iter().skip(1) {
                if r > (r_gn + r_gp + 2.0 * r_cutoff_bohr) {
                    break;
                }
                let t = distance(&grid_point, &center_coordinates_bohr[i]);
                r_gn = r_gn.min(t);
                r_gi.push((t, i));
            }

            if r_gn < (r_gn + r_cutoff_bohr) {
                r_gi.sort_by(|a, b| a.partial_cmp(b).unwrap());

                let mut polyhedron_functions = vec![1.0; num_centers];

                for &(r_i, i) in r_gi.iter() {
                    if r_i > (r_gn + r_cutoff_bohr) {
                        break;
                    }
                    for &(r_j, j) in r_gi.iter() {
                        if j > i {
                            if r_j > (r_i + r_cutoff_bohr) {
                                break;
                            }

                            let r_ij =
                                distance(&center_coordinates_bohr[i], &center_coordinates_bohr[j]);

                            let mu_ij = mu(r_i, r_j, r_ij);

                            // let s_ij = 0.5 - 0.5 * g(mu_ij);
                            let s_ij = 0.5 - 0.5 * z(mu_ij);
                            let s_ji = 1.0 - s_ij;

                            polyhedron_functions[i] *= s_ij;
                            polyhedron_functions[j] *= s_ji;
                        }
                    }
                }

                let s =
                    polyhedron_functions.iter().sum::<f64>() - polyhedron_functions[center_index];

                polyhedron_functions[center_index] / s
            } else {
                1.0
            }
        })
        .collect();

    weights
}
