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


class Resource(Enum):
    ArcanePower = auto()
    Rage = auto()
    Wrath = auto()
    Hatred = auto()
    Discipline = auto()
    Spirit = auto()
    Mana = auto()


########################### Gear ##################################
# Gear is basically just a group of Attribute modifiers
# in this perfect world, every piece of equipment is indestructable 
Gear = namedtuple('Gear', ['name', 'stats', 'slot', 'gtype'])


class GearSlot(Enum):
    MainHand = auto()
    OffHand = auto()
    OneHand = auto()
    TwoHand = auto()
    Head = auto()
    Shoulder = auto()
    Amulet = auto()
    Torso = auto()
    Waist = auto()
    Hand = auto()
    Foot = auto()
    Leg = auto()
    Ring = auto()
    

class GearType(Enum):
    OneHandedAxe = auto()
    OneHandedSword = auto()
    # many more to add

Fulminator = Gear(
    name='Fulminator',
    stats=AttrMod({
    }),
    slot=GearSlot.MainHand,
    gtype=GearType.OneHandedSword,
)


################## Class and Role specification ###################
MainAttrGain = namedtuple('MainAttrGain', ['s', 'd', 'i', 'v'])

def make_attr_mod(mag):
    return AttrMod({
        Attr.Strength: mag.s,
        Attr.Dexterity : mag.d,
        Attr.Intelligence : mag.i,
        Attr.Vitality : mag.v
    })
    

class Role(ABC):

    def __init__(
        self,
        basic_attrs,
        mag_upto60,
        mag_upto65,
        mag_upto70,
        level,
    ):
        self.basic_attrs = basic_attrs
        self.attr_mod_upto60 = make_attr_mod(mag_upto60)
        self.attr_mod_upto65 = make_attr_mod(mag_upto65)
        self.attr_mod_upto70 = make_attr_mod(mag_upto70)
        self.level = 1

        while self.level < level:
            self.level_up()


    def level_up(self):
        if self.level < 70:
            if 1 <= self.level <= 60:
                self.basic_attrs.update_many(self.attr_mod_upto60)
            elif 61 <= self.level <= 65:
                self.basic_attrs.update_many(self.attr_mod_upto65)
            else:
                self.basic_attrs.update_many(self.attr_mod_upto70)
            return True
        return False

    @abstractmethod
    def role(self):
        pass

    @property
    def level(self):
        return self.level

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
    

class RClass(Enum):
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


class Barbarian(Role):

    INIT_STATS = BasicAttrs({
        Attr.Strength : 35,
        Attr.Dexterity : 12,
        Attr.Intelligence : 10,
        Attr.Vitality : 25,
    })

    UPTO60 = MainAttrGain(s=4, d=1, i=1, v=5)
    UPTO65 = MainAttrGain(s=8, d=2, i=1, v=7)
    UPTO70 = MainAttrGain(s=10, d=2, i=1, v=9)

    def __init__(self, level=1):
        super(Barbarian, self).__init__(
            Barbarian.INIT_STATS, 
            Barbarian.UPTO60,
            Barbarian.UPTO65,
            Barbarian.UPTO70,
            level,
        )

    @property
    def role(self):
        return RClass.Barbarian


class DemonHunter(Role):

    INIT_STATS = BasicAttrs({
        Attr.Strength : 20, 
        Attr.Dexterity: 30,
        Attr.Intelligence: 15,
        Attr.Vitality : 20
    })

    UPTO60 = MainAttrGain(s=2, d=4, i=1, v=3)
    UPTO65 = MainAttrGain(s=3, d=8, i=2, v=5)
    UPTO70 = MainAttrGain(s=4, d=10, i=2, v=7)

    def __init__(self, level=1):
        super(DemonHunter, self).__init__(
            DemonHunter.INIT_STATS,
            DemonHunter.UPTO60,
            DemonHunter.UPTO65,
            DemonHunter.UPTO70,
            level,
        )

    @property
    def role(self):
        return RClass.DemonHunter


class Wizard(Role):

    INIT_STATS = BasicAttrs({
        Attr.Strength : 10, 
        Attr.Dexterity : 10,
        Attr.Intelligence : 40,
        Attr.Vitality : 15
    })

    UPTO60 = MainAttrGain(s=1, d=2, i=4, v=2)
    UPTO65 = MainAttrGain(s=2, d=2, i=7, v=3)
    UPTO70 = MainAttrGain(s=2, d=3, i=12, v=5)

    def __init__(self, level=1):
        super(Wizard, self).__init__(
            Wizard.INIT_STATS,
            Wizard.UPTO60,
            Wizard.UPTO65,
            Wizard.UPTO70,
            level,
        )

    @property
    def role(self):
        return RClass.Wizard

Character = namedtuple('Character', ['name', 'role'])
