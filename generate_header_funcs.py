import re
import os
import argparse

function_pattern = r'[\w\s]* \w+\([\w\s,]*\) {'
static_kw = r'^static'


def get_args():
    parser = argparse.ArgumentParser(description='Generate a header file with all non-static function signatures. \
                                                    **WARNING** WILL NOT INCLUDE EXTERNAL VARIABLES.')
    parser.add_argument('--src', type=str, help='source file', required=True)
    parser.add_argument('--outdir', type=str, help='output directory', required=True)
    return parser.parse_args()


def get_function_signatures(fname):

    with open(fname, 'r') as f:
        lines = f.readlines()

    signatures = [line.strip()[:len(line) - 3] + ';' for line in lines if re.search(function_pattern, line)]
    signatures = [signature for signature in signatures if not re.search(static_kw, signature)]
    return signatures


def parse_file_base_name(fpath):
    fname = fpath.split('/')[-1]
    fname = fname.split('.')[0]
    return fname


def generate_header_guards(base_name):
    header_guard_open = '_' + base_name + '_H_'
    header_guard_close = '/* ' + header_guard_open + ' */'
    return [f'#ifndef {header_guard_open}\n#define {header_guard_open}',
            f'#endif {header_guard_close}']
    

def generate_raw_header_file(base_name, function_signatures):
    header_guards = generate_header_guards(base_name)
    header_file_raw = header_guards[0] + '\n'*2 + \
                        '\n'.join(function_signatures) + \
                        '\n'*2 + header_guards[1]
    return header_file_raw


def output_header_file(outdir, base_name, raw_header_file):
    fname = base_name + '.h'
    fpath = os.path.join(outdir, fname)
    
    with open(fpath, 'w') as f:
        f.write(raw_header_file)


if __name__ == '__main__':
    args = get_args()
    src = args.src
    out = args.outdir    
    base_name = parse_file_base_name(src)
    function_signatures = get_function_signatures(src)
    raw_header = generate_raw_header_file(base_name, function_signatures)
    output_header_file(out, base_name, raw_header)

