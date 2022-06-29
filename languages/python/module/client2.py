import mod

def init_mod():
    mod.set_global('by client2')


def get_global():
    return mod.mod_global


init_mod()
