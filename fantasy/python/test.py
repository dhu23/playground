import unittest
from d3core import *
from items import *
from classes import *


########################## test items ################################
test_claymore = Item(
    name='Claymore',
    stats=[
        AttrAdd(attr=Attr.WeaponDamage, val=15),
        AttrAdd(attr=Attr.Strength, val=12),
    ],
    iconfig=WConfig(wtype=WType.Sword, handle=WHandle.TwoHanded),
)

test_leather_hood0 = Item(
    name='Leather Hood',
    stats=[
        AttrAdd(attr=Attr.Armor, val=20),
        AttrAdd(attr=Attr.Strength, val=10),
        AttrAdd(attr=Attr.Vitality, val=5),
    ],
    iconfig=AType.Helm,
)

test_cloth_tunic = Item(
    name='Cloth Tunic',
    stats=[
        AttrAdd(attr=Attr.Armor, val=15),
        AttrMul(attr=Attr.Armor, val=0.1),
    ],
    iconfig=AType.ChestArmor,
)

test_mystery_pants = Item(
    name='Mystery Pants',
    stats=[
        AttrAdd(attr=Attr.Armor, val=10),
        AttrMul(attr=Attr.Armor, val=0.2),
    ],
    iconfig=AType.Pants,
)

test_mighty_weapon = Item(
    name='Mighty weapon1',
    stats=[
        AttrAdd(attr=Attr.WeaponDamage, val=15),
        AttrAdd(attr=Attr.RageBonus, val=7),
    ],
    iconfig=WConfig(wtype=WType.MightWeapon, handle=WHandle.TwoHanded)
)

test_long_sword = Item(
    name='Long Sword',
    stats=[
        AttrAdd(attr=Attr.WeaponDamage, val=5),
    ],
    iconfig=WConfig(wtype=WType.Sword, handle=WHandle.OneHanded),
)

test_short_sword = Item(
    name='Short Sword',
    stats=[
        AttrAdd(attr=Attr.WeaponDamage, val=3),
    ],
    iconfig=WConfig(wtype=WType.Sword, handle=WHandle.OneHanded),
)

test_kite_shield = Item(
    name='Kite Shield',
    stats=[
        AttrAdd(attr=Attr.Armor, val=10),
    ],
    iconfig=AType.Shield,
)


class BasicAttrsTest(unittest.TestCase):

    def test_eq(self):

        a0 = BasicAttrs({
            Attr.Strength : 5, 
            Attr.Dexterity: 6, 
            Attr.Intelligence : 0, 
            Attr.Vitality : 0,
            Attr.LifeBonus : 0.0,
            Attr.CritChance : 0.0,
            Attr.CritDamage : 1.0,
            Attr.Block : 0.11,
            Attr.BlockAmount : 0, 
            Attr.LifePerRound : 0, 
            Attr.LifePerHit : 0, 
            Attr.LifePerKill : 0,
        })

        a1 = BasicAttrs({
            Attr.Strength : '5', 
            Attr.Dexterity : 6,
            Attr.Block: 0.11,
        })

        a2 = BasicAttrs({
            Attr.Strength : 5, 
            Attr.Dexterity : '6', 
            Attr.Block: '0.11',
        })

        #print(a0)
        #print(a1)

        self.assertEqual(a1, a0)
        self.assertEqual(a2, a0)
        self.assertEqual(a1, a2)

    def test_attr_update(self):

        a0 = BasicAttrs({
            Attr.Strength : 20
        })

        a0.update_one(Attr.Strength, make_attr_mod(5))
        self.assertEqual(a0[Attr.Strength], 25)

        a0.update_many({
            Attr.Strength : make_attr_mod(5),
            Attr.Dexterity : make_attr_mod(1),
        })

        self.assertEqual(a0[Attr.Strength], 30)
        self.assertEqual(a0[Attr.Dexterity], 1)
    

    def test_attr_mod(self):

        a0 = BasicAttrs({
            Attr.Strength : 27,
            Attr.Dexterity: 6, # 6 * 1.12 -> 6
            Attr.Intelligence : 30, # 0 + 30
            Attr.Vitality : 0, 
            Attr.LifeBonus : 0.21, # 0.1 + 0.11
            Attr.CritChance : 0.0,
            Attr.CritDamage : 1.3,
            Attr.Block : 0.0,
            Attr.BlockAmount : 0, 
            Attr.LifePerRound : 0, 
            Attr.LifePerHit : 7, # 2 + 5 -> 7
            Attr.LifePerKill : -6, # 4 - 10 -> -6
        })

        a1 = BasicAttrs({
            Attr.Strength : 5, 
            Attr.Dexterity : 6,
            Attr.CritDamage : 1.3,
            Attr.LifeBonus : 0.11,
            Attr.LifePerHit : 2,
            Attr.LifePerKill : 4,
        })

        m = {
            Attr.Strength : make_attr_mod(compound=5.5), # 550%
            Attr.Dexterity : make_attr_mod(compound=1.12), # 112%
            Attr.Intelligence : make_attr_mod(add=30),
            Attr.LifeBonus : make_attr_mod(add=0.1),
            Attr.LifePerHit : make_attr_mod(add=5),
            Attr.LifePerKill : make_attr_mod(add=-10),
        }
        
        # print(repr(a0))
        # print(repr(a1.mod_many(m)))

        self.assertEqual(a0, a1.mod_many(m))
        self.assertNotEqual(a0, a1)
        self.assertEqual(a0, a1.update_many(m))
        self.assertEqual(a0, a1)


class EquipmentTests(unittest.TestCase):

    def setUp(self):
        self.EQUIPMENT = Equipment(
            weapon_setup=TwoHandedSetup(test_claymore),
            armor_setup={
                ASlot.Head : test_leather_hood0,
                ASlot.Torso : test_cloth_tunic,
                ASlot.Legs : test_mystery_pants,
            },
            jewelry_setup=None,
        )

    def test_switch_two_handed(self):
        claymore_setup = two_handed_setup(test_claymore)
        
        mighty_setup = switch_main_weapon(claymore_setup, test_mighty_weapon)
        self.assertEqual(mighty_setup.weapon, test_mighty_weapon)

        sword_no_shield_setup = switch_main_weapon(claymore_setup, test_long_sword)
        self.assertEqual(sword_no_shield_setup.main, test_long_sword)
        self.assertEqual(sword_no_shield_setup.off, None)

        dual_sword_setup = switch_offhand(sword_no_shield_setup, test_short_sword)
        self.assertEqual(dual_sword_setup.main, test_long_sword)
        self.assertEqual(dual_sword_setup.off, test_short_sword)

        sword_board_setup = switch_offhand(dual_sword_setup, test_kite_shield)
        self.assertEqual(sword_board_setup.main, test_long_sword)
        self.assertEqual(sword_board_setup.off, test_kite_shield)

        back_claymore_setup = switch_main_weapon(sword_board_setup, test_claymore)
        self.assertEqual(back_claymore_setup, claymore_setup)


    def test_attr_mod_accumulation(self):
        ret = {}
        accumulate_item_attr_mod(self.EQUIPMENT, ret)
        self.assertEqual(
            ret[Attr.WeaponDamage], AttrMod(add=15, mul=None, compound=None))
        self.assertEqual(
            ret[Attr.Strength], AttrMod(add=22, mul=None, compound=None))
        self.assertEqual(
            ret[Attr.Armor], AttrMod(add=45, mul=0.3, compound=None))
        self.assertEqual(
            ret[Attr.Vitality], AttrMod(add=5, mul=None, compound=None))


class CharacterBasicTests(unittest.TestCase):
    def setUp(self):
        self.b1 = Barbarian('Koric', level=10)

    def test_basic_attr(self):
        self.assertEqual(self.b1.strength, 71)
        self.assertEqual(self.b1.dexterity, 21)
        self.assertEqual(self.b1.intelligence, 19)
        self.assertEqual(self.b1.vitality, 70)
        self.assertEqual(self.b1.max_hp, 2450)
        self.assertEqual(self.b1.armor, 71)
        self.assertEqual(self.b1.dodge, 0.021)
        self.assertEqual(self.b1.hp, 2450)

        # armor mitigation = 71/(500+71)
        self.assertEqual(self.b1.armor_mitigation, 0.1243)
        self.assertEqual(self.b1.res_mitigation(School.Physical), 0.0)
        self.assertEqual(self.b1.mitigate_damage(School.Physical), 0.1243)
        self.assertEqual(self.b1.max_resource(Resource.Rage), 100)

    def test_with_equipments(self):
        _equipment = Equipment(
            weapon_setup=TwoHandedSetup(test_mighty_weapon),
            armor_setup=None,
            jewelry_setup=None,
        )
        self.b1.equip(_equipment)
        self.assertEqual(self.b1.max_resource(Resource.Rage), 107)

        self.assertEqual(self.b1.strength, 71)
        self.assertEqual(self.b1.enhanced_damage, 0.71)
        self.assertEqual(self.b1.weapon_damage, 15)
        self.assertEqual(self.b1.actual_damage, 25) # 1.71 * 15 -> 25.65

        test_ring = Item(
            name='Test Ring 1',
            stats=[
                AttrAdd(attr=Attr.Strength, val=29),
            ],
            iconfig=AType.Ring,
        )

        self.b1.equip_right_ring(test_ring)
        self.assertEqual(self.b1.strength, 100)
        self.assertEqual(self.b1.enhanced_damage, 1.0)
        self.assertEqual(self.b1.weapon_damage, 15)
        self.assertEqual(self.b1.actual_damage, 30)

        test_ring2 = Item(
            name='Test Ring 2',
            stats=[
                AttrAdd(attr=Attr.WeaponDamage, val=14),
            ],
            iconfig=AType.Ring,
        )

        self.b1.equip_right_ring(test_ring2) # test_ring is taken off
        self.assertEqual(self.b1.strength, 71)
        self.assertEqual(self.b1.weapon_damage, 29)

        self.b1.equip_left_ring(test_ring) # wearing both rings
        self.assertEqual(self.b1.strength, 100)
        self.assertEqual(self.b1.weapon_damage, 29)
        self.assertEqual(self.b1.actual_damage, 58)



if __name__ == '__main__':
    unittest.main()
