pub fn floats_are_same(f1: f64, f2: f64) -> bool {
    let d = f1 - f2;
    return d.abs() < 1.0e-15;
}
