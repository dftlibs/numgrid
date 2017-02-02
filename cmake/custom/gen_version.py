import sys


def write_header(version, output_file_name):
    with open(output_file_name, 'w') as f:
        f.write('''#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

const char *PROGRAM_VERSION = "{0}";

#endif /* VERSION_H_INCLUDED */
'''.format(version))


def main():
    version_file_name = sys.argv[-2]
    output_file_name = sys.argv[-1]

    with open(version_file_name, 'r') as f:
        version = f.read().strip()

    write_header(version, output_file_name)


if __name__ == '__main__':
    main()
