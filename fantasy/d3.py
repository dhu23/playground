from enum import (
    Enum, 
    auto,
)
from collections import namedtuple, UserDict
from functools import singledispatch
from abc import ABC, abstractmethod


class School(Enum):
    Physical = auto()
    Fire = auto()
    Cold = auto()
    Lightning = auto()
    Arcane = auto()
    Poison = auto()
    Holy = auto()


############### generic value modifiers in functional forms ################
def identity(x):
    return x


def const(val):
    def inner():
        return val
    return inner


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



#################   Attribute, Role and their modifiers    ###############
class Attr(Enum):

    # basic attributes
    Strength = auto()
    Dexterity = auto()
    Intelligence = auto()
    Vitality = auto()
    LifeBonus = auto()
    CritChance = auto()
    CritDamage = auto()
    Block = auto()
    BlockAmount = auto()
    LifePerRound = auto()
    LifePerHit = auto()
    LifePerKill = auto()
    
    # derived attributes (affected by other attributes)
    HP = auto() # Vitality + LifeBonus => HP
    Armor = auto() # Str/Dex + Role => Armor
    Dodge = auto() # Dex + Role => Dodge
    WeaponDamage = auto() # Str/Dex/Int + Role => WeaponDmg
    PhysicalRes = auto() # by Int
    FireRes = auto() # by Int
    ColdRes = auto() # by Int
    LightningRes = auto() # by Int
    ArcaneRes = auto() # by Int
    PoisonRes = auto() # by Int
    HolyRes = auto() # by Int


AttrInfo = namedtuple('AttrInfo', ['init', 'conv'])


def two_decimal_float(x):
    return round(float(x), 2)


_int_type = AttrInfo(init=int, conv=int)
_float_type = AttrInfo(init=float, conv=two_decimal_float)


_ATTRIBUTE_INFO_MAP = {

    # basic attributes
    Attr.Strength : _int_type,
    Attr.Dexterity : _int_type,
    Attr.Intelligence : _int_type,
    Attr.Vitality : _int_type,
    Attr.LifeBonus : AttrInfo(init=const(1.0), conv=two_decimal_float),
    Attr.CritChance : _float_type,
    Attr.CritDamage : AttrInfo(init=const(1.0), conv=two_decimal_float),
    Attr.Block : _float_type,
    Attr.BlockAmount : _int_type, 
    Attr.LifePerRound : _int_type,
    Attr.LifePerHit : _int_type,
    Attr.LifePerKill : _int_type,

    # derived attributes (affected by other attributes)
    Attr.HP : _int_type, # modifed by BaseHP and Vitality and LifeBonus
    Attr.Armor : _int_type, # modified by Str/Dex
    Attr.Dodge : _float_type, # modifed by Dex
    Attr.WeaponDamage : _int_type, # modifed by Str/Dex/Int
    Attr.PhysicalRes : _float_type, # modified by Int
    Attr.FireRes : _float_type, # modified by Int
    Attr.ColdRes : _float_type, # modified by Int
    Attr.LightningRes : _float_type, # modified by Int
    Attr.ArcaneRes : _float_type, # modified by Int
    Attr.PoisonRes : _float_type, # modified by Int
    Attr.HolyRes : _float_type, # modified by Int
}


class UnknownAttr(RuntimeError):
    def __repr__(self):
        return "UnknownAttr(s): %s" % str(self.args)


def attr_func(attr):
    if attr not in _ATTRIBUTE_INFO_MAP:
        raise UnknownAttr(attr)
    return _ATTRIBUTE_INFO_MAP[attr]


_BASIC_ATTRS = {
    Attr.Strength, 
    Attr.Dexterity,
    Attr.Intelligence,
    Attr.Vitality,
    Attr.LifeBonus,
    Attr.CritChance,
    Attr.CritDamage,
    Attr.Block,
    Attr.BlockAmount, 
    Attr.LifePerRound,
    Attr.LifePerHit,
    Attr.LifePerKill,
}


class UnknownBasicAttr(RuntimeError):
    def __repr__(self):
        return "UnknownBasicAttr(s): %s" % str(self.args)


def assert_basic_attr(attr):
    if attr not in _BASIC_ATTRS:
        raise UnknownBasicAttr(attr)


class AttrMod(UserDict):
    def __init__(self, data):
        # would fail on bad attr
        _data = {}
        for attr, v in data.items():
            _ = attr_func(attr) # for attr validation
            _data[attr] = v
        super(AttrMod, self).__init__(_data)


class BasicAttrs(UserDict):
    '''
    it tracks basic attributes and has getter functions for derived attributes
    '''

    def __str__(self):
        return str(self.data)

    def __init__(self, data):
        # would fail on bad attributes
        _data = {}
        for attr, v in data.items():
            assert_basic_attr(attr)
            _data[attr] = attr_func(attr).conv(v)

        for attr in _BASIC_ATTRS:
            if attr not in _data:
                _data[attr] = attr_func(attr).init()

        super(BasicAttrs, self).__init__(_data)

    def __getitem__(self, attr):
        assert_basic_attr(attr)
        return self.data[attr]

    def __eq__(self, other):
        assert isinstance(other, BasicAttrs), \
            'cannot use __eq__ between BasicAttrs and %s' % type(other)    
        for attr in _BASIC_ATTRS:
            if self[attr] != other[attr]: 
                return False
        return True

    def mod_by(self, attr_mod):
        for attr, f in attr_mod.items():
            assert_basic_attr(attr)
            self.data[attr] = f(self.data[attr])
        return self


    #def __iadd__(self, other):
    #    assert isinstance(other, BasicAttrs), \
    #        'cannot add BasicAttrs and %s' % type(other)
    #    for k in list(Attr):
    #        self[k] += other[k]
    #    return self


    #def __add__(self, other):
    #    ret = BasicAttrs()
    #    ret += self
    #    ret += other
    #    return ret



class Resource(Enum):
    ArcanePower = auto()
    Rage = auto()
    Wrath = auto()
    Hatred = auto()
    Discipline = auto()
    Spirit = auto()
    Mana = auto()


class Role(Enum):
    Barbarian = auto()
    Crusader = auto()
    Wizard = auto()
    Monk = auto()
    WitchDoctor = auto()
    Necromancer = auto()
    DemonHunter = auto()


class UnknownRole(RuntimeError):
    def __repr__(self):
        return 'Unknown role %s' % str(self.args)


def assert_role(role):
    if role not in set(Role):
        raise UnknownRole(role)


class Role(ABC):

    def __init__(self, basic_attrs, level=1):

        self.basic_attrs = basic_attrs
        self.level = level

    @abstractmethod
    def level_up(self):
        pass

    # getters for attributes
    @property
    def strength(self):
        return self.basic_attrs[Attr.Strength]

    @property
    def dexterity(self):
        return self.basic_attrs[Attr.Dexterity]

    @property
    def intelligence(self):
        return self.basic_attrs[Attr.Intelligence]

    @property
    def vitality(self):
        return self.basic_attrs[Attr.Vitality]

    @property
    def lifeBonus(self):
        return self.basic_attrs[Attr.LifeBonus]

    @property
    def critChance(self):
        return self.basic_attrs[Attr.CritChance]

    @property
    def critDamage(self):
        return self.basic_attrs[Attr.CritDamage]

    @property
    def block(self):
        return self.basic_attrs[Attr.Block]

    @property
    def blockAmount(self):
        return self.basic_attrs[Attr.BlockAmount]

    @property
    def lifePerRound(self):
        return self.basic_attrs[Attr.LifePerRound]

    @property
    def lifePerHit(self):
        return self.basic_attrs[Attr.LifePerHit]

    @property
    def lifePerKill(self):
        return self.basic_attrs[Attr.LifePerKill]
    

class Barbarian(Role):

    INIT_STATS = BasicAttrs({
        Attr.Strength : 35,
        Attr.Dexterity : 12,
        Attr.Intelligence : 10,
        Attr.Vitality : 25,
    })

    def __init__(self):
        super(Barbarian, self).__init__(Role.INIT_STATS)

    def level_up(self):
        self
    

Character = namedtuple('Character', ['name', 'role'])
