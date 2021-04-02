import unittest
from d3core import *
from items import *
from classes import *

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
            weapon_setup=TwoHandedSetup(test_two_handed_sword0),
            armor_setup={
                ASlot.Head : test_leather_hood0,
                ASlot.Torso : test_cloth_tunic,
                ASlot.Legs : test_mystery_pants,
            },
            jewelry_setup=None,
        )

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


if __name__ == '__main__':
    unittest.main()
