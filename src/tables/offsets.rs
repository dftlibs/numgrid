use std::collections::HashMap;

pub fn offsets() -> HashMap<usize, usize> {
    let mut mapping = HashMap::new();
    mapping.insert(6, 0);
    mapping.insert(14, 6);
    mapping.insert(26, 20);
    mapping.insert(38, 46);
    mapping.insert(50, 84);
    mapping.insert(74, 134);
    mapping.insert(86, 208);
    mapping.insert(110, 294);
    mapping.insert(146, 404);
    mapping.insert(170, 550);
    mapping.insert(194, 720);
    mapping.insert(230, 914);
    mapping.insert(266, 1144);
    mapping.insert(302, 1410);
    mapping.insert(350, 1712);
    mapping.insert(434, 2062);
    mapping.insert(590, 2496);
    mapping.insert(770, 3086);
    mapping.insert(974, 3856);
    mapping.insert(1202, 4830);
    mapping.insert(1454, 6032);
    mapping.insert(1730, 7486);
    mapping.insert(2030, 9216);
    mapping.insert(2354, 11246);
    mapping.insert(2702, 13600);
    mapping.insert(3074, 16302);
    mapping.insert(3470, 19376);
    mapping.insert(3890, 22846);
    mapping.insert(4334, 26736);
    mapping.insert(4802, 31070);
    mapping.insert(5294, 35872);
    mapping.insert(5810, 41166);
    mapping
}
