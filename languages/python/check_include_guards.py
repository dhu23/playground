import os
import sys

def get_include_guard(file_name):
    lineOne, lineTwo = None, None
    with open(file_name, 'r') as f:
        for l in f:
            if lineOne and lineTwo: break

            l1 = l.strip()
            if l1.startswith('#ifndef'):
                lineOne = l1.split()[1].strip()
            elif l1.startswith('#define'):
                lineTwo = l1.split()[1].strip()
    return lineOne, lineTwo

def check_include_guards_in_dir(dir_path):
    guards_set = set()
    for r, d, f in os.walk(dir_path):
        for each in f:
            if not each.endswith('.h'): continue
            file_name = os.path.join(r, each)
            g1, g2 = get_include_guard(file_name)
            
            print('file=%s,g1=%s,g2=%s' % (each, g1, g2))
            
            if g1 != g2:
                print('WARN:Include guard mismatch %s vs %s' % (g1, g2))
            if g1 in guards_set:
                print('WARN:already seen %s' % g1)
            guards_set.add(g1)
            


if __name__ == "__main__":
    folder = sys.argv[1]
    check_include_guards_in_dir(folder)
