from enum import (
    Enum, 
    auto,
)
from collections import namedtuple, UserDict
from functools import singledispatch

class School(Enum):
    Physical = auto()
    Fire = auto()
    Cold = auto()
    Lightning = auto()
    Arcane = auto()
    Poison = auto()
    Holy = auto()


class Resource(Enum):
    ArcanePower = auto()
    Rage = auto()
    Wrath = auto()
    Hatred = auto()
    Discipline = auto()
    Spirit = auto()
    Mana = auto()


############### generic value modifiers in functional forms ################
def identity(x):
    return x


def limit(val, floor=None, cap=None):
    if cap is not None and val > cap:
        return cap
    if floor is not None and val < floor:
        return floor
    return val


def add_by(val, floor=None, cap=None):
    def inner(x):
        return limit(x + val, floor=floor, cap=cap)
    return inner


def mul_by(val, floor=None, cap=None):
    def inner(x):
        return limit(x * val, floor=floor, cap=cap)
    return inner



#####################   Attribute and their modifiers    ###################
class Attr(Enum):
    Strength = auto()
    Dexterity = auto()
    Intelligence = auto()
    Vitality = auto()
    Armor = auto()
    WeaponDamage = auto()
    Dodge = auto()
    Block = auto()
    BlockAmount = auto()
    LifeBonus = auto()
    LifePerRound = auto()
    LifePerHit = auto()
    LifePerKill = auto()


AttrInfo = namedtuple('AttrInfo', ['init', 'conv'])

int_type_info = AttrInfo(init=int, conv=int)

float_type_info = AttrInfo(init=float, conv=lambda x : round(float(x), 2))

_ATTRIBUTE_DEFAULT_VAL_FUNCS = {
    Attr.Strength : int_type_info,
    Attr.Dexterity : int_type_info,
    Attr.Intelligence : int_type_info,
    Attr.Vitality : int_type_info,
    Attr.Armor : int_type_info,
    Attr.WeaponDamage : int_type_info,
    Attr.Dodge : float_type_info,
    Attr.Block : float_type_info,
    Attr.BlockAmount : int_type_info,
    Attr.LifeBonus : float_type_info,
    Attr.LifePerRound : int_type_info,
    Attr.LifePerHit : int_type_info,
    Attr.LifePerKill : int_type_info,
}

def attr_func(stat_type):
    if stat_type not in _ATTRIBUTE_DEFAULT_VAL_FUNCS:
        raise UnknownAttr(stat_type)
    return _ATTRIBUTE_DEFAULT_VAL_FUNCS[stat_type]


class UnknownAttr(RuntimeError):
    def __repr__(self):
        return "UnknownStat(s): %s" % str(self.args)



class RoleAttrs(UserDict):

    def __str__(self):
        return str(self.data)


    def __init__(self, data):
        # would fail on bad attributes
        _data = dict(
            (k, attr_func(k)[1](v)) 
            for k, v in data.items())

        for k in list(Attr):
            if k not in _data:
                _data[k] = attr_func(k)[0]()

        super(RoleAttrs, self).__init__(_data)


    def get_stat(self, stat_type):
        assert stat_type in list(Attr), \
            'unknown stat type %s' % str(stat_type)
        return self[stat_type]


    def __eq__(self, other):
        assert isinstance(other, RoleAttrs), \
            'cannot use __eq__ between RoleAttrs and %s' % type(other)    
        for k in list(Attr):
            if self[k] != other[k]: 
                return False
        return True


    def __iadd__(self, other):
        assert isinstance(other, RoleAttrs), \
            'cannot add RoleAttrs and %s' % type(other)
        for k in list(Attr):
            self[k] += other[k]
        return self


    def __add__(self, other):
        ret = RoleAttrs({})
        ret += self
        ret += other
        return ret


class AttrMod(UserDict):
    def __init__(self, data):
        # would fail on bad attr
        _data = {}
        for k, v in data.items():
            _ = attr_func(k) # for attr validation
            _data[k] = v
        super(AttrMod, self).__init__(_data)


def mod_attr(role_attrs, attr_mod):
    ret_dict = {}
    for k, v in role_attrs.items():
        mod_func = attr_mod.get(k, identity)
        ret_dict[k] = mod_func(v)
    return RoleAttrs(ret_dict)



Character = namedtuple('Character', ['name', 'role'])
