use crate::tables;

pub fn get_angular_grid(num_points: usize) -> (Vec<(f64, f64, f64)>, Vec<f64>) {
    let offsets = tables::offsets::offsets();

    let offset: usize = match offsets.get(&num_points) {
        Some(v) => *v,
        None => panic!("get_angular_grid called with unsupported num_points"),
    };

    (
        tables::coordinates::COORDINATES[offset..(offset + num_points)].to_vec(),
        tables::weights::WEIGHTS[offset..(offset + num_points)].to_vec(),
    )
}
