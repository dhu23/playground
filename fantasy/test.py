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
            Attr.LifeBonus : 1.0,
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

        a0.update_one(Attr.Strength, add_by(5))
        self.assertEqual(a0[Attr.Strength], 25)

        a0.update_many(
            AttrMod({
                Attr.Strength : add_by(5),
                Attr.Dexterity : add_by(1),
            })
        )

        self.assertEqual(a0[Attr.Strength], 30)
        self.assertEqual(a0[Attr.Dexterity], 1)
    

    def test_attr_mod(self):

        a0 = BasicAttrs({
            Attr.Strength : 20, # 5 * 5.1215 -> 20(cap)
            Attr.Dexterity: 6, # 6.72906 -> 6
            Attr.Intelligence : 30, # 0 + 30
            Attr.Vitality : 0, 
            Attr.LifeBonus : 0.21, # 0.1 + 0.11
            Attr.CritChance : 0.0,
            Attr.CritDamage : 1.3,
            Attr.Block : 0.0,
            Attr.BlockAmount : 0, 
            Attr.LifePerRound : 0, 
            Attr.LifePerHit : 4, # 2 + 5 -> 4 (cap)
            Attr.LifePerKill : 0, # 4 - 10 -> 0 (floor)
        })

        a1 = BasicAttrs({
            Attr.Strength : 5, 
            Attr.Dexterity : 6,
            Attr.CritDamage : 1.3,
            Attr.LifeBonus : 0.11,
            Attr.LifePerHit : 2,
            Attr.LifePerKill : 4,
        })

        m = AttrMod({
            Attr.Strength : mul_by(5.1215, cap=20),
            Attr.Dexterity : mul_by(1.12151),
            Attr.Intelligence : add_by(30),
            Attr.LifeBonus : add_by(0.1),
            Attr.LifePerHit : add_by(5, cap=4),
            Attr.LifePerKill : add_by(-10, floor=0),
        })
        
        self.assertEqual(a0, a1.mod_many(m))
        self.assertNotEqual(a0, a1)
        self.assertEqual(a0, a1.update_many(m))
        self.assertEqual(a0, a1)


class EquipmentTests(unittest.TestCase):
    
    def test_equipment_ctor(self):

        two_handed_sword = Item(
            name='Scottish Claymore',
            stats=AttrMod({
                Attr.WeaponDamage : const(15),
                Attr.Strength : add_by(12), 
            }),
            iconfig=WConfig(wtype=WType.Sword, handle=WHandle.TwoHanded),
        )

        leather_hood = Item(
            name='Leather Hood',
            stats=AttrMod({
                Attr.Armor : add_by(20),
                Attr.Strength : add_by(10),
                Attr.Vitality : add_by(5),
            }),
            iconfig=AType.Helm,
        )

        e = Equipment(
            weapon_setup=TwoHandedSetup(weapon=two_handed_sword),
            armor_setup={
                ASlot.Head : leather_hood,
            },
            jewelry=JewelrySetUp(amulet=None, left=None, right=None),
        )



class CharacterBasicTests(unittest.TestCase):
    def test_attr_level_up(self):

        b1 = Barbarian('Koric', level=10)
        self.assertEqual(b1.strength, 71)
        self.assertEqual(b1.dexterity, 21)
        self.assertEqual(b1.intelligence, 19)
        self.assertEqual(b1.vitality, 70)




if __name__ == '__main__':
    unittest.main()
