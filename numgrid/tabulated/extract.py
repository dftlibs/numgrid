import sys
from collections import defaultdict

l_map = {
    's': 0,
    'p': 1,
    'd': 2,
    'f': 3,
    'g': 4,
    'h': 5,
}


def parse_file(file_name):

    exponents_charge = defaultdict(list)
    exponents_block = defaultdict(list)
    max_l = defaultdict(int)

    with open(file_name, 'r') as f:

        lines = f.read().splitlines()

        # remove empty lines
        lines = filter(lambda line: line != '', lines)

        # remove empty lines and comments except the lines that tell
        # us which l quantum number we are dealing with
        def _keep(line):
            if line.endswith('functions'):
                return True
            if line.startswith('$'):
                return False
            return True
        lines = filter(_keep, lines)

        try:
            while True:
                line = next(lines)
                if 'functions' in line:
                    l_quantum_number = l_map[line.split()[1]]
                    max_l[charge] = max(l_quantum_number, max_l[charge])
                    exponents_in_this_l_block = []
                    line = next(lines)
                    num_exponents = int(line.split()[0])
                    for _ in range(num_exponents):
                        e = float(next(lines))
                        exponents_charge[charge].append(e)
                        exponents_block[(charge, l_quantum_number)].append(e)
                else:
                    charge = int(line.split()[-1])
        except StopIteration:
            pass

    return exponents_charge, exponents_block, max_l


def print_function(return_type,
                   function_name,
                   data,
                   return_value):
    print(f'\n{return_type} {function_name}(const int charge)')
    print('{')
    print('    switch (charge)')
    print('    {')
    for k, v in data.items():
        print(f'    case {k}:')
        if isinstance(v, list):
            v_strings = [str(x) for x in v]
            _v = ', '.join(v_strings)
            print(f'        return {{{_v}}};')
        else:
            print(f'        return {v};')
    print('    default:')
    print(f'        NUMGRID_ERROR("No entry found in {function_name}");')
    print('    }')
    print(f'    return {return_value};')
    print('}')


if __name__ == '__main__':
    file_name = sys.argv[-1]
    exponents_charge, exponents_block, max_l = parse_file(file_name)

    alpha_min = {}
    alpha_max = {}
    for charge in exponents_charge:
        alpha_max[charge] = max(exponents_charge[charge])
        _exponents = []
        for l in range(max_l[charge] + 1):
            _exponents.append(min(exponents_block[(charge, l)]))
        alpha_min[charge] = _exponents

    print('#include "tabulated_exponents.h"')
    print('#include "error_handling.h"')

    print_function('std::vector<double>',
                   'alpha_min_tabulated',
                   alpha_min,
                   '{0.0}')
    print_function('double',
                   'alpha_max_tabulated',
                   alpha_max,
                   '0.0')
    print_function('int',
                   'max_l_quantum_number_tabulated',
                   max_l,
                   '0')
