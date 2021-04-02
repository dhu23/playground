from enum import (
    Enum, 
    auto,
)
from collections import namedtuple, UserDict
from functools import singledispatch
from abc import ABC, abstractproperty, abstractmethod
from pprint import pformat


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


###################### Attributes and their modifiers #######################
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

    # class-related attributes
    RageBonus = auto()
    WrathBonus = auto()
    SpiritBonus = auto()
    HatredBonus = auto()
    DisciplineBonus = auto()
    ArcanePowerBonus = auto()
    ManaBonus = auto()


BasicAttrInfo = namedtuple('BasicAttrInfo', ['init', 'conv'])


def two_decimal_float(x):
    return round(float(x), 2)

def four_decimal_float(x):
    return round(float(x), 4)

_int_type = BasicAttrInfo(init=int, conv=int)
_float_type = BasicAttrInfo(init=float, conv=two_decimal_float)


# for an attribute x, 
# and a list of AttrAdd values, a1, a2, ...
# and a list of AttrMul values, b1, b2, ...
# and a list of CompoundingMul values, c1, c2, ...
# the final attribute would be 
# (x + a1 + a2 + ... ) * (1 + b1 + b2 + ... ) * c1 * c2 * ... 
AttrAdd = namedtuple('AttrAdd', ['attr', 'val'])
AttrMul = namedtuple('AttrMul', ['attr', 'val'])
CompoundingMul = namedtuple('CompoundingMul', ['attr', 'val'])

AttrMod = namedtuple('AttrMod', ['add', 'mul', 'compound'])

def make_attr_mod(add=None, mul=None, compound=None):
    return AttrMod(add=add, mul=mul, compound=compound)


def apply_attr_mod(attr_mod, val):
    if attr_mod is None:
        return val

    add, mul, comp = attr_mod
    ret = val
    if add is not None:
        ret += add
    if mul is not None:
        ret *= (1+mul)
    if comp is not None:
        ret *= comp
    return ret
    


@singledispatch
def accumulate_item_attr_mod(x, ret): # populate the data in ret with AttrMod
    raise RuntimeError('does not support basic attr mod:%s' % type(x))


@accumulate_item_attr_mod.register(AttrAdd)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        addnew = x.val if add is None else x.val + add
        ret[x.attr] = AttrMod(add=addnew, mul=mul, compound=compound)
    else:
        ret[x.attr] = AttrMod(add=x.val, mul=None, compound=None)


@accumulate_item_attr_mod.register(AttrMul)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        mulnew = x.val if mul is None else two_decimal_float(x.val + mul)
        ret[x.attr] = AttrMod(add=add, mul=mulnew, compound=compound)
    else:
        ret[x.attr] = AttrMod(add=None, mul=x.val, compound=None)


@accumulate_item_attr_mod.register(CompoundingMul)
def _(x, ret):
    if x.attr in ret:
        add, mul, compound = ret[x.attr]
        compnew = x.val if compound is None else two_decimal_float(x.val * compound)
        ret[x.attr] = AttrMod(add=add, mul=mul, compound=compnew)
    else:
        ret[x.attr] = AttrMod(add=None, mul=None, compound=x.val)


class BasicAttrs(UserDict):
    '''
    it tracks basic attributes and has getter functions for derived attributes.
    This class does not apply capping/flooring logic to the atributes. Instead
    all the capping/flooring are done at Character level so that we retain the 
    flexibility of different 
    '''
    INFO = {
        # basic attributes
        Attr.Strength : _int_type,
        Attr.Dexterity : _int_type,
        Attr.Intelligence : _int_type,
        Attr.Vitality : _int_type,
        Attr.WeaponDamage : _int_type, 
        Attr.LifeBonus : BasicAttrInfo(init=const(0.0), conv=two_decimal_float),
        Attr.CritChance : _float_type,
        Attr.CritDamage : BasicAttrInfo(init=const(1.0), conv=two_decimal_float),
        Attr.Block : _float_type,
        Attr.BlockAmount : _int_type, 
        Attr.LifePerRound : _int_type,
        Attr.LifePerHit : _int_type,
        Attr.LifePerKill : _int_type,
    }

    @staticmethod
    def get_func(attr):
        return BasicAttrs.INFO[attr]

    def __str__(self):
        return str(self.data)

    def __repr__(self):
        return 'BasicAttrs\n' + pformat(self.data, indent=2)

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
        return self.data[attr]

    def __eq__(self, other):
        assert isinstance(other, BasicAttrs), \
            'cannot use __eq__ between BasicAttrs and %s' % type(other)
        for attr in BasicAttrs.INFO:
            if self[attr] != other[attr]: 
                return False
        return True

    def update_one(self, attr, attr_mod):
        try:
            _, conv = BasicAttrs.get_func(attr)
        except:
            return self
        # print(attr_mod)
        self.data[attr] = conv(apply_attr_mod(attr_mod, self.data[attr]))
        return self

    def update_many(self, attr_mod_dict):
        for attr, attr_mod in attr_mod_dict.items():
            self.update_one(attr, attr_mod)
        return self

    def mod_one(self, attr, mod_func):
        ret = self.copy()
        ret.update_one(attr, mod_func)
        return ret

    def mod_many(self, attr_mod_dict):
        ret = self.copy()
        ret.update_many(attr_mod_dict)
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


@accumulate_item_attr_mod.register(Item)
def _(x, ret):
    if x.stats:
        for each in x.stats:
            accumulate_item_attr_mod(each, ret)

def _accumulate_none(x, ret):
    return ret

accumulate_item_attr_mod.register(type(None), _accumulate_none)

# WeaponSetup instances
TwoHandedSetup = namedtuple('TwoHandedSetup', ['weapon'])
# dual wielding includes sword+board setup
DualWieldingSetup = namedtuple('DualWieldingSetup', ['main', 'off'])


@accumulate_item_attr_mod.register(TwoHandedSetup)
def _(x, ret):
    accumulate_item_attr_mod(x.weapon, ret)


@accumulate_item_attr_mod.register(DualWieldingSetup)
def _(x, ret):
    accumulate_item_attr_mod(x.main, ret)
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


def dual_wielding_setup(mainhand=None, offhand=None):
    if mainhand is not None:
        _assert_one_handed_weapon(mainhand)
    if offhand is not None:
        try:
            _assert_one_handed_weapon(offhand)
        except:
            _assert_offhand(offhand)
    return DualWieldingSetup(main=mainhand, off=offhand)


@singledispatch
def switch_main_weapon(x, weapon):
    raise RuntimeError(
        'cannot switch weapon from %s to %s' % (type(x), type(weapon)))


@switch_main_weapon.register(TwoHandedSetup)
def _(x, weapon):
    if weapon.iconfig.whandle == WHandle.TwoHanded:
        return two_handed_setup(weapon)
    if weapon.iconfig.whandle == WHandle.OneHanded:
        return dual_wielding_setup(main=weapon, off=None)
    raise RuntimeError('cannot switch two handed setup to %s' % type(weapon))


@switch_main_weapon.register(DualWieldingSetup)
def _(x, weapon):
    if weapon.iconfig.whandle == WHandle.TwoHanded:
        return two_handed_setup(weapon)
    if weapon.iconfig.whandle == WHandle.OneHanded:
        return dual_wielding_setup(main=weapon, off=x.off)
    raise RuntimeError('cannot switch dual wielding setup to %s' % type(weapon))


@singledispatch
def switch_off_weapon(x, offhand):
    raise RuntimeError(
        'cannot switch weapon from %s to %s' % (type(x), type(weapon)))


@switch_off_weapon.register(TwoHandedSetup)
def _(x, offhand):
    _assert_offhand(offhand)
    return dual_wielding_setup(main=None, off=offhand)


@switch_off_weapon.register(DualWieldingSetup)
def _(x, offhand):
    _assert_offhand(offhand)
    return dual_wielding_setup(main=x.main, off=offhand)


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
                weapon_setup, (TwoHandedSetup, DualWieldingSetup)), \
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

    def equip_mainhand(self, weapon):
        try:
            self.weapon_setup = switch_main_weapon(weapon)
            return True
        except:
            print('failed to switch to %s' % str(weapon))
            return False

    def equip_offhand(self, offhand):
        try:
            self.weapon_setup = switch_off_weapon(self.weapon_setup, weapon)
            return True
        except:
            print('failed to switch to %s' % str(offhand))
            return False

    def equip_armor(self, armor_piece):
        self.armor_setup[armor_piece.iconfig] = armor_piece
        return True

    def equip_left_ring(self, ring):
        pass

    def equip_right_ring(self, ring):
        pass

    def equip_amulet(self, amulet):
        pass


@accumulate_item_attr_mod.register(Equipment)
def _(x, ret):
    accumulate_item_attr_mod(x.weapon_setup, ret)
    if x.armor_setup:
        for _, armor_piece in x.armor_setup.items():
            accumulate_item_attr_mod(armor_piece, ret)
    accumulate_item_attr_mod(x.jewelry_setup, ret)


############################ Damage/Heal #############################
class School(Enum):
    Physical = auto()
    Fire = auto()
    Cold = auto()
    Lightning = auto()
    Arcane = auto()
    Poison = auto()
    Holy = auto()


Dmg = namedtuple('Dmg', ['amount', 'school'])

def mk_dmg(val, school):
    if val is None:
        return None
    else:
        return Dmg(amount=val, school=school)

DamageResult = namedtuple('DamageResult', ['caused', 'overkill'])
HealResult = namedtuple('HeadResult', ['healed', 'overheal'])

################# skill/runes and resource management  ###################
#class Skill(Enum):
#    Bash = auto()
#
#
#class Rune(Enum):
#    # Bash
#    Frostbite = auto()
#    Onslaught = auto()
#    Punish = auto()
#    Instigation = auto()
#    Pulverize = auto()

class Resource(Enum):
    ArcanePower = auto()
    Rage = auto()
    Wrath = auto()
    Hatred = auto()
    Discipline = auto()
    Spirit = auto()
    Mana = auto()


class Skill(ABC):

    @abstractproperty
    def name(self):
        pass
    
    @abstractproperty
    def cost(self):
        pass

    @abstractmethod
    def use(self, character):
        pass


##################### Character and Role specification ####################
MainAttrGain = namedtuple('MainAttrGain', ['s', 'd', 'i', 'v'])

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

    SCHOOL_MAPPING = {
        School.Physical : Attr.PhysicalRes,
        School.Fire: Attr.FireRes,
        School.Cold : Attr.ColdRes,
        School.Lightning : Attr.LightningRes,
        School.Arcane : Attr.ArcaneRes,
        School.Poison : Attr.PoisonRes,
        School.Holy : Attr.HolyRes,
    }

    RESOURCE_BONUS_MAPPING = {
        Resource.Rage : Attr.RageBonus,
        Resource.Wrath : Attr.WrathBonus,
        Resource.Spirit : Attr.SpiritBonus,
        Resource.Hatred : Attr.HatredBonus,
        Resource.Discipline : Attr.DisciplineBonus,
        Resource.ArcanePower : Attr.ArcanePowerBonus, 
        Resource.Mana : Attr.ManaBonus,
    }

    def __init__(
        self,
        level,
        basic_attrs,
        mag_upto60,
        mag_upto65,
        mag_upto70,
        baseline_resource,
        equipment=None,
        effects=None,
        # skill_set,
    ):
        def make_attr_mods(mag):
            return {
                Attr.Strength : make_attr_mod(add=mag.s),
                Attr.Dexterity : make_attr_mod(add=mag.d),
                Attr.Intelligence : make_attr_mod(add=mag.i),
                Attr.Vitality : make_attr_mod(add=mag.v),
            }

        self._level = 1
        self.basic_attrs = basic_attrs
        self.baseline_resource = baseline_resource # never changes

        self.attr_mod_upto60 = make_attr_mods(mag_upto60)
        self.attr_mod_upto65 = make_attr_mods(mag_upto65)
        self.attr_mod_upto70 = make_attr_mods(mag_upto70)
        self.level_to(level)

        self.equipment = equipment
        self.effects = effects
        
        # full hp at character construction
        self._hp = self.max_hp
        
        self._resource = {}
        for _res_type, val in self.baseline_resource.items():
            # full resource at character construction
            self._resource[_res_type] = self.max_resource(_res_type)
        

    def _cap_hp(self):
        if self._hp > self.max_hp:
            self._hp = self.max_hp

    def _cap_resource(self):
        for res, _ in self.baseline_resource.items():
            max_res = self.max_resource(res)
            if self._resource[res] > max_res:
                self._resource[res] = max_res
        
    def equip(self, equipment):
        self.equipment = equipment
        self._cap_hp()
        self._cap_resource()

    @property
    def attr_mod_dict(self):
        ret = {}
        if self.equipment:
            accumulate_item_attr_mod(self.equipment, ret)
        if self.effects:
            accumulate_item_attr_mod(self.effects, ret)
        return ret

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

    def reduce_hp(self, delta):
        if delta > 0: # receiving damage
            hp = self._hp
            dmg = abs(delta)
            if hp <= delta:
                self._hp = 0
                return (hp, dmg-hp)
            else:
                self._hp = hp-dmg
                return (dmg, None)
        return None

    def take_damage(self, dmg):
        amt, school = dmg
        delta = int(amt * (1-self.mitigate(school)))
        _caused, _overkill = self.reduce_hp(-delta)

        return DamageResult(
            caused=mk_dmg(_caused, school),
            overkill=mk_dmg(_overkill, school),
        )

    @property
    def is_alive(self):
        return self._hp > 0

    @abstractproperty
    def role(self):
        pass

    @abstractproperty
    def resource_types(self):
        pass

    @property
    def hp(self):
        return self._hp

    @property
    def max_hp(self):
        '''
        hp value depends on vitality and lifeBonus
        '''
        if 1 <= self.level <= 60:
            _hp_per_vit = 35
        elif 61 <= self.level <= 65:
            _hp_per_vit = 55
        else:
            _hp_per_vit = 100
        return int((self.vitality * _hp_per_vit) * (1 + self.lifeBonus))

    @property
    def armor(self):
        ret = apply_attr_mod(
            self.attr_mod_dict.get(Attr.Armor, None),
            self.strength)
        return int(ret)

    def mitigate(self, protection):
        return four_decimal_float(protection / (protection + 50*self.level))

    @property
    def armor_mitigation(self):
        return self.mitigate(self.armor)

    @property
    def dodge(self):

        def _dodge_pct_from_dex(dex):
            if dex <= 100:
                return dex*0.1
            elif dex <= 500:
                return 100*0.1 + (dex-100)*0.025
            elif dex <= 1000:
                return 100*0.1 + 400*0.025 + (dex-500)*0.02
            else:
                return 100*0.1 + 400*0.025 + 500*0.02 + (dex-1000)*0.01

        ret = apply_attr_mod(
            self.attr_mod_dict.get(Attr.Dodge, None),
            _dodge_pct_from_dex(self.dexterity)*0.01)
        return four_decimal_float(ret)

    def _get_res(self, res_attr):
        ret = apply_attr_mod(self.attr_mod_dict.get(res_attr, None), 0)
        return int(ret)

    def mitigate_damage(self, school):
        '''
        both armor and school resistance apply to mitigate incoming damage
        assume the mitigation is armor% and res%, 
        then the amount damage taken can be calculated as 
        taken%=(1-armor%)*(1-res%)
        therefore the migitation is 1-taken% = armor% + res% - armor%*res%
        '''
        _armor = self.armor_mitigation
        _res = self.res_mitigation(school)
        return four_decimal_float(_armor + _res - _armor * _res)

    def school_res(self, school):
        res_attr = Character.SCHOOL_MAPPING[school]
        return self._get_res(res_attr)

    @property
    def physical_res(self):
        return self._get_res(Attr.PhysicalRes)

    @property
    def fire_res(self):
        return self._get_res(Attr.FireRes)

    @property
    def cold_res(self):
        return self._get_res(Attr.ColdRes)

    @property
    def lightning_res(self):
        return self._get_res(Attr.LightningRes)

    @property
    def arcane_res(self):
        return self._get_res(Attr.ArcaneRes)

    @property
    def poison_res(self):
        return self._get_res(Attr.PoisonRes)

    @property
    def holy_res(self):
        return self._get_res(Attr.HolyRes)

    def res_mitigation(self, school):
        return self.mitigate(self.school_res(school))

    @property
    def physical_mitigation(self):
        return self.mitigate(self.physical_res)

    @property
    def fire_mitigation(self):
        return self.mitigate(self.fire_res)

    @property
    def cold_mitigation(self):
        return self.mitigate(self.cold_res)

    @property
    def lightning_mitigation(self):
        return self.mitigate(self.lightning_res)

    @property
    def arcane_mitigation(self):
        return self.mitigate(self.arcane_res)

    @property
    def poison_mitigation(self):
        return self.mitigate(self.poison_res)

    @property
    def holy_mitigation(self):
        return self.mitigate(self.holy_res)

    @property
    def level(self):
        return self._level

    # getters for basic attributes
    def _get_basic_attr(self, attr):
        ret = apply_attr_mod(
            self.attr_mod_dict.get(attr, None),
            self.basic_attrs[attr])
        return BasicAttrs.get_func(attr).conv(ret)

    @property
    def strength(self):
        return self._get_basic_attr(Attr.Strength)

    @property
    def dexterity(self):
        return self._get_basic_attr(Attr.Dexterity)

    @property
    def intelligence(self):
        return self._get_basic_attr(Attr.Intelligence)

    @property
    def vitality(self):
        return self._get_basic_attr(Attr.Vitality)

    @property
    def lifeBonus(self):
        return self._get_basic_attr(Attr.LifeBonus)

    @property
    def critChance(self):
        return self._get_basic_attr(Attr.CritChance)

    @property
    def critDamage(self):
        return self._get_basic_attr(Attr.CritDamage)

    @property
    def block(self):
        return self._get_basic_attr(Attr.Block)

    @property
    def blockAmount(self):
        return self._get_basic_attr(Attr.BlockAmount)

    @property
    def lifePerRound(self):
        return self._get_basic_attr(Attr.LifePerRound)

    @property
    def lifePerHit(self):
        return self._get_basic_attr(Attr.LifePerHit)

    @property
    def lifePerKill(self):
        return self._get_basic_attr(Attr.LifePerKill)

    def max_resource(self, resource_type):
        resource_bonus_type = Character.RESOURCE_BONUS_MAPPING[resource_type]
        ret = apply_attr_mod(
            self.attr_mod_dict.get(resource_bonus_type, None),
            self.baseline_resource[resource_type])
        return int(ret)

    def update_resource(self, resource_type, delta):
        _res = self._resource[resource_type] + delta
        if _res < 0:
            self._resource[resource_type] = 0
        elif _res >= self.max_resource(resource_type):
            self._resource[resource_type] = self.max_resource(resource_type)
        else:
            self._resource[resource_type] = _res

    def cast(self, skill_name):
        _skill = self.skill_set(skill_name)
        _cost = _skill.cost
        if _cost > self._resource[_skill.resource]:
            # cannot use the skill due to insufficient resource
            raise RuntimeError('cannot use the skill')
        else:
            # use skill 
            self.update_resource(_skill.resource, _cost)
            return _skill.use(self)


# use :: Skill -> Character -> [Runes] -> Effects


########################## Game/World mechanism ###########################
# tracking effects, such as DoTs, debuffs and anything timed outside of 
# characters, so that Character can provide a more functional definition of 
# the game rule and timed effects (that might involves the external world),
# are all tracked at this one place. This can make concurrency easier.

class EffectType(Enum):
    DoT = auto()
    Buff = auto()
    Debuff = auto()

Damage = namedtuple('Dmg', ['name', 'dmg'])
DoT = namedtuple('DoT', ['name', 'duration', 'dmg'])


class RoundBasedInstance(object):
    '''round based game instance'''
    def __init__(self):
        self.objects = {}


class RealTimeInstance(object):
    '''real time based game instance'''
