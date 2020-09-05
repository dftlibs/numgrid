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
