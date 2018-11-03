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

file_name = 'dyall.v3z'
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

for charge in exponents_charge:
    print(charge, max_l[charge], max(exponents_charge[charge]))
    for l in range(max_l[charge] + 1):
        print(l, min(exponents_block[(charge, l)]))
