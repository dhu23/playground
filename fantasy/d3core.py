from enum import (
    Enum, 
    auto,
)
from collections import namedtuple, UserDict
from functools import singledispatch
from abc import ABC, abstractproperty


############### generic value modifiers in functional forms ################
def identity(x):
    return x


def const(val):
    def inner():
        return val
    return inner


def _limit(val, floor=None, cap=None):
    if cap is not None and val > cap:
        return cap
    if floor is not None and val < floor:
        return floor
    return val


def add_by(val, floor=None, cap=None):
    def inner(x):
        return _limit(x + val, floor=floor, cap=cap)
    return inner


def mul_by(val, floor=None, cap=None):
    def inner(x):
        return _limit(x * val, floor=floor, cap=cap)
    return inner


#################   Attribute, Role and their modifiers    ###############
class Attr(Enum):

    # basic attributes
    Strength = auto()
    Dexterity = auto()
    Intelligence = auto()
    Vitality = auto()
    WeaponDamage = auto()
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
    Dodge = auto() # Dex + Role => Dodge%
    EnhanceDamage = auto() # Str/Dex/Int + Role => Enhance dmg%
    ActualDamage = auto() # Damage + Enhance damage + Role/skill => ActualDmg 
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


BasicAttrInfo = namedtuple('BasicAttrInfo', ['init', 'conv'])


def two_decimal_float(x):
    return round(float(x), 2)


_int_type = BasicAttrInfo(init=int, conv=int)
_float_type = BasicAttrInfo(init=float, conv=two_decimal_float)


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
        Attr.WeaponDamage : _int_type, 
        Attr.LifeBonus : BasicAttrInfo(init=const(1.0), conv=two_decimal_float),
        Attr.CritChance : _float_type,
        Attr.CritDamage : BasicAttrInfo(init=const(1.0), conv=two_decimal_float),
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


########################### Items/Equipment ##############################
# Gear is basically just a group of Attribute modifiers
# in this perfect world, every piece of equipment is indestructable 


class ASlot(Enum):
    Head = auto()
    Shoulders = auto()
    Torso = auto()
    Wrists = auto()
    Hands = auto()
    Waist = auto()
    Legs = auto()
    Feet = auto()
    Jewelry = auto()
    OffHand = auto()


# imagine in Haskell, the data would be defined as 
# data Head = Helm | SpiritStone | VoodooMask | WizardHat
class AType(Enum): # Armor Type
    # head
    Helm = auto()
    SpiritStone = auto()
    VoodooMask = auto()
    WizardHat = auto()
    # shoulders
    Pauldron = auto()
    # Torso
    ChestArmor = auto()
    Cloak = auto()
    # Wrists
    Bracers = auto()
    # Hands
    Gloves = auto()
    # Waist
    Belt = auto()
    MightyBelt = auto()
    # Legs
    Pants = auto()
    # Feet
    Boots = auto()
    # Jewelry
    Amulet = auto()
    Ring = auto()
    # OffHand
    Shield = auto()
    CrusaderShield = auto()
    Mojo = auto()
    Orb = auto()
    Quiver = auto()
    Phylactery = auto()


ARMOR_TYPE_MAPPING = {
    AType.Helm : ASlot.Head, 
    AType.SpiritStone : ASlot.Head, 
    AType.VoodooMask : ASlot.Head,
    AType.WizardHat : ASlot.Head,
    AType.Pauldron : ASlot.Shoulders,
    AType.ChestArmor : ASlot.Torso,
    AType.Cloak : ASlot.Torso,
    AType.Bracers : ASlot.Wrists,
    AType.Gloves : ASlot.Hands,
    AType.Belt : ASlot.Waist,
    AType.MightyBelt : ASlot.Waist,
    AType.Pants : ASlot.Legs,
    AType.Boots : ASlot.Feet,
    AType.Amulet : ASlot.Jewelry,
    AType.Ring : ASlot.Jewelry,
    AType.Shield : ASlot.OffHand,
    AType.CrusaderShield : ASlot.OffHand,
    AType.Mojo : ASlot.OffHand,
    AType.Orb : ASlot.OffHand,
    AType.Quiver : ASlot.OffHand,
    AType.Phylactery : ASlot.OffHand,
}


def _get_armory_slot_type(armor_type):
    return ARMOR_TYPE_MAPPING[armor_type]


class WType(Enum): # Weapon Type
    Axe = auto()
    Dagger = auto()
    Mace = auto()
    Spear = auto()
    Sword = auto()
    CeremonialKnife = auto()
    FistWeapon = auto()
    Flail = auto()
    MightWeapon = auto()
    Scythe = auto()
    Polearm = auto()
    Staff = auto()
    Daibo = auto()
    Bow = auto()
    Crossbow = auto()
    HandCrossbow = auto()
    Wand = auto()


class WHandle(Enum): # weapon handness, 1h or 2h etc
    OneHanded = auto()
    TwoHanded = auto()


class WRange(Enum): # weapon range
    Melee = auto()
    Ranged = auto()


def _get_weapon_range(weapon_type):
    assert weapon_type in list(WType), \
        'Unknown weapon type %s' % type(weapon_type)
    if weapon_type in [
            WType.Bow, WType.Crossbow, WType.HandCrossbow, WType.Wang]:
        return WRange.Ranged
    else:
        return WRange.Melee


WConfig = namedtuple('WConfig', ['wtype', 'handle'])
# iconfig should be either WConfig or AType
Item = namedtuple('Item', ['name', 'stats', 'iconfig'])


AttrAdd = namedtuple('AttrAdd', ['attr', 'val'])
AttrMul = namedtuple('AttrMul', ['attr', 'val'])
CompoundingMul = namedtuple('CompoundingMul', ['attr', 'val'])

ItemAttrMod = namedtuple('ItemAttrMod', ['add', 'mul', 'compound'])


@singledispatch
def accumulate_item_attr_mod(x, ret): # populate the data in ret
    raise RuntimeError('does not support basic attr mod:%s' % type(x))


@accumulate_item_attr_mod.register(AttrAdd)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        addnew = x.val if add is None else x.val + add
        ret[x.attr] = ItemAttrMod(add=addnew, mul=mul, compound=compound)
    else:
        ret[x.attr] = ItemAttrMod(add=x.val, mul=None, compound=None)


@accumulate_item_attr_mod.register(AttrMul)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        mulnew = x.val if mul is None else two_decimal_float(x.val + mul)
        ret[x.attr] = ItemAttrMod(add=add, mul=mulnew, compound=compound)
    else:
        ret[x.attr] = ItemAttrMod(add=None, mul=x.val, compound=None)


@accumulate_item_attr_mod.register(CompoundingMul)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        compnew = x.val if compound is None else two_decimal_float(x.val * compound)
        ret[x.attr] = ItemAttrMod(add=add, mul=mul, compound=compnew)
    else:
        ret[x.attr] = ItemAttrMod(add=None, mul=None, compound=x.val)


@accumulate_item_attr_mod.register(Item)
def _(x, ret):
    if x.stats:
        for each in x.stats:
            accumulate_item_attr_mod(each, ret)


# WeaponSetup instances
TwoHandedSetup = namedtuple('TwoHandedSetup', ['weapon'])
DualWielding = namedtuple('DualWielding', ['main', 'off'])
OneHandedSetup = namedtuple('OneHandedSetup', ['weapon', 'off'])


@accumulate_item_attr_mod.register(TwoHandedSetup)
def _(x, ret):
    accumulate_item_attr_mod(x.weapon, ret)


@accumulate_item_attr_mod.register(DualWielding)
def _(x, ret):
    accumulate_item_attr_mod(x.main, ret)
    accumulate_item_attr_mod(x.off, ret)


@accumulate_item_attr_mod.register(OneHandedSetup)
def _(x, ret):
    accumulate_item_attr_mod(x.weapon, ret)
    accumulate_item_attr_mod(x.off, ret)


def _assert_two_handed_weapon(weapon):
    assert weapon.iconfig.handle == WHandle.TwoHanded, \
        'not a 2h weapon: %s' % weapon


def _assert_one_handed_weapon(weapon):
    assert weapon.iconfig.handle == WHandle.OneHanded, \
        'not a 1h weapon: %s' % weapon


def _assert_offhand(armor):
    assert armor.iconfig in [
        AType.Shield, 
        AType.CrusaderShield,
        AType.Mojo,
        AType.Orb,
        AType.Quiver,
        AType.Phylactery,
    ], 'not an offhand item %s' % armor


def two_handed_setup(weapon):
    _assert_two_handed_weapon(weapon)
    return TwoHandedSetup(weapon=weapon)


def dual_wielding_setup(mainhand, offhand):
    _assert_one_handed_weapon(mainhand)
    _assert_two_handed_weapon(offhand)
    return DualWielding(main=mainhand, off=offhand)


def one_handed_and_sheild_setup(weapon, offhand):
    _assert_one_handed_weapon(weapon)
    _assert_offhand(offhand)
    return OneHandedSetup(weapon=weapon, off=offhand)


JewelrySetup = namedtuple('JewelrySetup', ['amulet', 'left', 'right'])

def _assert_amulet(amulet):
    assert amulet.itype == AType.Amulet


def _assert_ring(ring):
    assert ring.itype == AType.Ring


def jewelry_setup(amulet=None, left_ring=None, right_ring=None):
    if amulet is not None:
        _assert_amulet(amulet)
    if left_ring is not None:
        _assert_ring(left_ring)
    if right_ring is not None:
        _assert_ring(right_ring)
    return JewelrySetUp(amulet=amulet, left=left_ring, right=right_ring)


@accumulate_item_attr_mod.register(JewelrySetup)
def _(x, ret):
    if x.amulet:
        accumulate_item_attr_mod(x.amulet, ret)
    if x.left:
        accumulate_item_attr_mod(x.left, ret)
    if x.right:
        accumulate_item_attr_mod(x.right, ret)


class Equipment(object):
    def __init__(self, weapon_setup=None, armor_setup=None, jewelry_setup=None):
        # NOTE in the future we can just support infinitely large inventory
        if weapon_setup is not None:
            assert isinstance(
                weapon_setup, (TwoHandedSetup, DualWielding, OneHandedSetup)), \
                'weapon setup %s is not accepted' % type(weapon_setup)
        self.weapon_setup = weapon_setup

        self.armor_setup = {}
        if armor_setup is not None:
            for k, v in armor_setup.items():
                assert k in list(ASlot), \
                    'Unknown armory slot %s' % type(k)
                self.armor_setup[k] = v

        for k in list(ASlot):
            if k not in self.armor_setup:
                self.armor_setup[k] = None

        self.jewelry_setup = jewelry_setup


@accumulate_item_attr_mod.register(Equipment)
def _(x, ret):
    accumulate_item_attr_mod(x.weapon_setup, ret)
    for _, armor_piece in x.armor_setup.items():
        if armor_piece is not None:
            accumulate_item_attr_mod(armor_piece, ret)
    accumulate_item_attr_mod(x.jewelry_setup, ret)


################# Damage, DoT, effects(buffs/debuffs)  ##################
class School(Enum):
    Physical = auto()
    Fire = auto()
    Cold = auto()
    Lightning = auto()
    Arcane = auto()
    Poison = auto()
    Holy = auto()


Damage = namedtuple('Damage', ['name', 'amount', 'school'])


class EffectType(Enum):
    DoT = auto()
    Buff = auto()
    Debuff = auto()

DoT = namedtuple('DoT', ['name', 'duration', 'dmg'])



################## Class and Role specification ###################
MainAttrGain = namedtuple('MainAttrGain', ['s', 'd', 'i', 'v'])

def make_attr_mod(mag):
    return AttrMod({
        Attr.Strength: add_by(mag.s),
        Attr.Dexterity : add_by(mag.d),
        Attr.Intelligence : add_by(mag.i),
        Attr.Vitality : add_by(mag.v),
    })
    

class Character(ABC):
    _ATTRIBUTE_INFO_MAP = {
    
        # derived attributes (affected by other attributes)
        Attr.HP : _int_type, # modifed by BaseHP and Vitality and LifeBonus
        Attr.Armor : _int_type, # modified by Str/Dex
        Attr.Dodge : _float_type, # modifed by Dex
        Attr.EnhanceDamage: _float_type, # modifed by Str/Dex/Int
        Attr.PhysicalRes : _float_type, # modified by Int
        Attr.FireRes : _float_type, # modified by Int
        Attr.ColdRes : _float_type, # modified by Int
        Attr.LightningRes : _float_type, # modified by Int
        Attr.ArcaneRes : _float_type, # modified by Int
        Attr.PoisonRes : _float_type, # modified by Int
        Attr.HolyRes : _float_type, # modified by Int
    }

    def __init__(
        self,
        level,
        basic_attrs,
        mag_upto60,
        mag_upto65,
        mag_upto70,
        # equipment,
        # skill_set,
    ):
        self._level = 1
        self.basic_attrs = basic_attrs
        self.attr_mod_upto60 = make_attr_mod(mag_upto60)
        self.attr_mod_upto65 = make_attr_mod(mag_upto65)
        self.attr_mod_upto70 = make_attr_mod(mag_upto70)
        # self.equipment = equipment
        # self.skill_set = skill_set
        # self.effects = {}
        # self.equipments = None

        self.level_to(level)

    def level_up(self):
        if self.level < 70:
            #print(self.attr_mod_upto60)
            if 1 <= self.level <= 60:
                self.basic_attrs.update_many(self.attr_mod_upto60)
            elif 61 <= self.level <= 65:
                self.basic_attrs.update_many(self.attr_mod_upto65)
            else:
                self.basic_attrs.update_many(self.attr_mod_upto70)
            self._level += 1
            return True
        return False

    def level_to(self, level):
        assert level <= 70, 'cannot level beyond lvl70'
        while self.level < level:
            self.level_up()

    @abstractproperty
    def role(self):
        pass

    @abstractproperty
    def hp(self):
        pass

    @abstractproperty
    def armor(self):
        pass

    @abstractproperty
    def dodge(self):
        pass

    @abstractproperty
    def enhance_damage(self):
        pass

    @abstractproperty
    def physical_res(self):
        pass

    @abstractproperty
    def fire_res(self):
        pass

    @abstractproperty
    def cold_res(self):
        pass

    @abstractproperty
    def lightning_res(self):
        pass

    @abstractproperty
    def arcane_res(self):
        pass

    @abstractproperty
    def poison_res(self):
        pass

    @abstractproperty
    def holy_res(self):
        pass

    @property
    def level(self):
        return self._level

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


class UnknownRClass(RuntimeError):
    def __repr__(self):
        return 'Unknown role %s' % str(self.args)


def assert_rclass(role):
    if role not in set(RClass):
        raise UnknownRClass(role)


class Resource(Enum):
    ArcanePower = auto()
    Rage = auto()
    Wrath = auto()
    Hatred = auto()
    Discipline = auto()
    Spirit = auto()
    Mana = auto()


