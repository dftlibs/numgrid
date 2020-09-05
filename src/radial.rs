#![allow(clippy::many_single_char_names)]

use crate::comparison;
use crate::parameters;
use statrs::function::gamma;

// TCA 106, 178 (2001), eq. 25
// we evaluate r_inner for s functions
fn get_r_inner(max_error: f64, alpha_inner: f64) -> f64 {
    let d = 1.9;

    let mut r = d - (1.0 / max_error).ln();
    r *= 2.0 / 3.0;
    r = r.exp() / alpha_inner;
    r = r.sqrt();

    r
}

#[test]
fn test_get_r_inner() {
    assert!(comparison::floats_are_same(
        get_r_inner(1.0e-12, 2.344e4),
        0.0000012304794589759454
    ));
    assert!(comparison::floats_are_same(
        get_r_inner(1.0e-12, 2.602e1),
        0.000036931719276091795
    ));
}

// TCA 106, 178 (2001), eq. 19
fn get_r_outer(max_error: f64, alpha_outer: f64, l: usize, guess: f64) -> f64 {
    let m = (2 * l) as f64;
    let mut r_old = std::f64::MAX;
    let mut step = 0.5;
    let mut sign = 1.0;
    let mut r = guess;

    while (r_old - r).abs() > parameters::SMALL {
        let c = gamma::gamma((m + 3.0) / 2.0);
        let a = (alpha_outer * r * r).powf((m + 1.0) / 2.0);
        let e = (-alpha_outer * r * r).exp();
        let f = c * a * e;

        let sign_old = sign;
        sign = if f > max_error { 1.0 } else { -1.0 };
        if r < 0.0 {
            sign = 1.0
        }
        if sign * sign_old < 0.0 {
            step *= 0.1;
        }

        r_old = r;
        r += sign * step;
    }

    r
}

#[test]
fn test_get_r_outer() {
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.3023, 0, 2.4),
        9.827704700468292
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.2753, 1, 2.4),
        10.97632862649149
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 1.1850, 2, 2.4),
        5.656909461147809
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.1220, 0, 1.4),
        15.470033591458682
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.7270, 1, 1.4),
        6.75449681779016
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.1220, 0, 1.4),
        15.470033591458682
    ));
    assert!(comparison::floats_are_same(
        get_r_outer(1.0e-12, 0.7270, 1, 1.4),
        6.75449681779016
    ));
}

// TCA 106, 178 (2001), eqs. 17 and 18
fn get_h(max_error: f64, l: usize, guess: f64) -> f64 {
    let m = (2 * l) as f64;
    let mut h_old = std::f64::MAX;
    let mut h = guess;
    let mut step = 0.1 * guess;
    let mut sign = -1.0;

    let pi = std::f64::consts::PI;

    while (h_old - h).abs() > parameters::SMALL {
        let c0 = 4.0 * (2.0 as f64).sqrt() * pi;
        let cm = gamma::gamma(3.0 / 2.0) / gamma::gamma((m + 3.0) / 2.0);
        let p0 = 1.0 / h;
        let e0 = (-pi * pi / (2.0 * h)).exp();
        let pm = (pi / h).powf(m / 2.0);
        let rd0 = c0 * p0 * e0;
        let f = cm * pm * rd0;

        let sign_old = sign;
        sign = if f > max_error { -1.0 } else { 1.0 };
        if h < 0.0 {
            sign = 1.0
        }
        if sign * sign_old < 0.0 {
            step *= 0.1;
        }

        h_old = h;
        h += sign * step;
    }

    h
}

#[test]
fn test_get_h() {
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 0, 0.9827703),
        0.1523549756417546
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 1, 1.0976330),
        0.1402889524647394
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 2, 1.0976330),
        0.13136472924710518
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 0, 1.5470000),
        0.15235497564177505
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 1, 1.5470000),
        0.14028895246475018
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 0, 1.5470000),
        0.15235497564177505
    ));
    assert!(comparison::floats_are_same(
        get_h(1.0e-12, 1, 1.5470000),
        0.14028895246475018
    ));
}
