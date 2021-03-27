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


class UnknownAttr(RuntimeError):
    def __repr__(self):
        return "UnknownAttr(s): %s" % str(self.args)


def assert_attr(attr):
    if attr not in list(Attr):
        raise UnknownAttr(attr)


AttrInfo = namedtuple('AttrInfo', ['init', 'conv'])


def two_decimal_float(x):
    return round(float(x), 2)


_int_type = AttrInfo(init=int, conv=int)
_float_type = AttrInfo(init=float, conv=two_decimal_float)


_ATTRIBUTE_INFO_MAP = {

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


class AttrMod(UserDict):
    def __init__(self, data):
        # would fail on bad attr
        _data = {}
        for attr, v in data.items():
            assert_attr(attr)
            _data[attr] = v
        super(AttrMod, self).__init__(_data)


class UnknownBasicAttr(RuntimeError):
    def __repr__(self):
        return "UnknownBasicAttr(s): %s" % str(self.args)


class BasicAttrs(UserDict):
    '''
    it tracks basic attributes and has getter functions for derived attributes
    '''
    INFO = {
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
    }

    @staticmethod
    def assert_attr(attr):
        if attr not in BasicAttrs.INFO:
            raise UnknownBasicAttr(attr)

    @staticmethod
    def get_func(attr):
        ret = BasicAttrs.INFO.get(attr)
        if ret is None:
            raise UnknownBasicAttr(attr)
        return ret

    def __str__(self):
        return str(self.data)

    def __init__(self, data):
        # would fail on bad attributes
        _data = {}
        for attr, v in data.items():
            _data[attr] = BasicAttrs.get_func(attr).conv(v)

        for attr in BasicAttrs.INFO:
            if attr not in _data:
                _data[attr] = BasicAttrs.get_func(attr).init()

        super(BasicAttrs, self).__init__(_data)

    def copy(self):
        return BasicAttrs(dict(self.data))

    def __getitem__(self, attr):
        BasicAttrs.assert_attr(attr)
        return self.data[attr]

    def __eq__(self, other):
        assert isinstance(other, BasicAttrs), \
            'cannot use __eq__ between BasicAttrs and %s' % type(other)
        for attr in BasicAttrs.INFO:
            if self[attr] != other[attr]: 
                return False
        return True

    def update_one(self, attr, mod_func):
        try:
            _, conv = BasicAttrs.get_func(attr)
        except:
            return self
        self.data[attr] = conv(mod_func(self.data[attr]))
        return self

    def update_many(self, attr_mod):
        for attr, f in attr_mod.items():
            self.update_one(attr, f)
        return self

    def mod_one(self, attr, mod_func):
        ret = self.copy()
        ret.update_one(attr, mod_func)
        return ret

    def mod_many(self, attr_mod):
        ret = self.copy()
        ret.update_many(attr_mod)
        return ret


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
