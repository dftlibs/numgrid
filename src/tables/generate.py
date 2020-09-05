import glob
import math


def read_data():
    coordinates = []
    ws = []
    offsets = []

    num_points_total = 0
    for file_name in sorted(glob.glob("lebedev_*.txt")):
        with open(file_name, "r") as f:
            num_points = 0
            for line in f:
                num_points += 1
                p, t, w = tuple(map(float, line.split()))
                coordinates.append((t, p))
                ws.append(w)
            offsets.append((num_points, num_points_total))
            num_points_total += num_points

    return coordinates, ws, offsets


def write_table_offsets(offsets):
    with open("offsets.rs", "w") as f:
        f.write("use std::collections::HashMap;\n")
        f.write("\npub fn offsets() -> HashMap<usize, usize> {\n")
        f.write("    let mut mapping = HashMap::new();\n")
        for (num_points, offset) in offsets:
            f.write(f"    mapping.insert({num_points}, {offset});\n")
        f.write("    mapping\n")
        f.write("}\n")


def fix(x):
    if abs(x) < 1.0e-15:
        return 0.0
    elif abs(abs(x) - 0.707106781186547) < 1.0e-15:
        if x > 0:
            return "F"
        else:
            return "-F"
    else:
        return x


def spherical_to_cartesian(coordinate):
    theta, phi = coordinate

    theta_rad = math.pi * theta / 180.0
    phi_rad = math.pi * phi / 180.0

    theta_sin = math.sin(theta_rad)
    theta_cos = math.cos(theta_rad)
    phi_sin = math.sin(phi_rad)
    phi_cos = math.cos(phi_rad)

    x = fix(theta_sin * phi_cos)
    y = fix(theta_sin * phi_sin)
    z = fix(theta_cos)

    return x, y, z


def write_tables(coordinates, ws):
    with open("coordinates.rs", "w") as f:
        f.write("#![allow(clippy::unreadable_literal)]\n\n")
        f.write("const F: f64 = std::f64::consts::FRAC_1_SQRT_2;\n\n")
        f.write(f"pub const COORDINATES: [(f64, f64, f64); {len(coordinates)}] = [\n")
        for (x, y, z) in coordinates:
            f.write(f"    ({x}, {y}, {z}),\n")
        f.write(f"];\n")

    with open("weights.rs", "w") as f:
        f.write("#![allow(clippy::unreadable_literal)]\n\n")
        f.write(f"pub const WEIGHTS: [f64; {len(ws)}] = [\n")
        for w in ws:
            f.write(f"    {w},\n")
        f.write(f"];\n")


if __name__ == "__main__":
    coordinates_spherical, ws, offsets = read_data()
    coordinates = list(map(spherical_to_cartesian, coordinates_spherical))

    write_table_offsets(offsets)

    write_tables(coordinates, ws)
