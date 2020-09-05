// JCP 88, 2547 (1988), eq. 20
#[inline]
fn f3(x: f64, becke_hardness: usize) -> f64 {
    let mut f = x;
    for _ in 0..becke_hardness {
        f *= 1.5 - 0.5 * f * f;
    }
    return f;
}

fn _floats_are_same(f1: f64, f2: f64) -> bool {
    let d = f1 - f2;
    return d.abs() < 1.0e-15;
}

#[test]
fn test_f3() {
    assert!(_floats_are_same(f3(-0.055317, 3), -0.18512279618232905));
    assert!(_floats_are_same(f3(-0.491055, 3), -0.97223000595777));
    assert!(_floats_are_same(f3(-0.974945, 3), -0.9999999999974357));
    assert!(_floats_are_same(f3(0.644797, 3), 0.9976980942759357));
    assert!(_floats_are_same(f3(0.878986, 3), 0.9999993429837151));
}
