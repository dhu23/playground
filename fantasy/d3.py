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


class Attribute(Enum):
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


ATTRIBUTE_DEFAULT_VAL_FUNCS = {
    Attribute.Strength : int,
    Attribute.Dexterity: int, 
    Attribute.Intelligence: int,
    Attribute.Vitality: int, 
    Attribute.Armor: int, 
    Attribute.WeaponDamage: int, 
    Attribute.Dodge : float,
    Attribute.Block : float,
    Attribute.BlockAmount : int,
    Attribute.LifeBonus : float,
    Attribute.LifePerRound : int,
    Attribute.LifePerHit : int,
    Attribute.LifePerKill : int,
}


class UnknownAttribute(RuntimeError):
    def __repr__(self):
        return "UnknownStat(s): %s" % str(self.args)


class RoleAttributes(UserDict):

    @staticmethod
    def default_val(stat_type):
        if stat_type not in ATTRIBUTE_DEFAULT_VAL_FUNCS:
            raise UnknownAttribute(stat_type)
        return ATTRIBUTE_DEFAULT_VAL_FUNCS[stat_type]()


    def __init__(self, data):
        data_keys = set(data.key())
        all_attributes = set(Attribute)

        unknown_attributes = data_keys - all_attributes
        if unknown_attributes:
            raise UnknownAttribute(unknown_attributes)

        _data = dict(data)
        for attri in all_attributes - data_keys:
            _data[attri] = RoleAttributes.default_val(attri)

        super(RoleStats, self).__init__(_data)


    def get_stat(self, stat_type):
        assert stat_type in list(Attribute), \
            'unknown stat type %s' % str(stat_type)
        return self[stat_type]
        

    def __add__(self, other):

        @singledispatch
        def add_attribute(x, ra):
            raise RuntimeError(
                'does not support adding by RoleAttributes for type %s' % type(x))

        @add_attribute(RoleAttributes)
        def add_attribute(x, ra):
            for attri in list(Attribute):
                ra[attri] += x[attri]

        @add_attribute(dict)
        def _(x, ra):
            add_attribute(RoleAttributes(x), ra)

        self += other


class Role(object):
    
    def __init__(self, skills, gears):
        self.skills = skills
        self.gears = gears


Character = namedtuple('Character', ['name', 'role'])
