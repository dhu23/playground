# create a module scope global variable

mod_global = None


def set_global(x):
    global mod_global
    print(f'overwriting {mod_global} with {x}')
    mod_global = x

