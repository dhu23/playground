import unittest
from d3 import *


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

        print(a0)
        print(a1)

        self.assertEqual(a1, a0)
        self.assertEqual(a2, a0)
        self.assertEqual(a1, a2)

    #def test_mod(self):

    #    a0 = BasicAttrs({
    #        Attr.Strength : 20, 
    #        Attr.Dexterity: 6, # 6.72906 -> 6
    #        Attr.Intelligence : 30, 
    #        Attr.Vitality : 0,
    #        Attr.Armor : 0,
    #        Attr.WeaponDamage : 0,
    #        Attr.Dodge : 0.43,
    #        Attr.Block : 0.0,
    #        Attr.BlockAmount : 0, 
    #        Attr.LifeBonus : 0.21, 
    #        Attr.LifePerRound : 0, 
    #        Attr.LifePerHit : 4, 
    #        Attr.LifePerKill : 0,
    #    })

    #    a1 = BasicAttrs({
    #        Attr.Strength : 5, 
    #        Attr.Dexterity : 6,
    #        Attr.Dodge : 0.35,
    #        Attr.LifeBonus : 0.11,
    #        Attr.LifePerHit : 2,
    #        Attr.LifePerKill : 4,
    #    })

    #    m = AttrMod({
    #        Attr.Strength : mul_by(5.1215, cap=20),
    #        Attr.Dexterity : mul_by(1.12151),
    #        Attr.Intelligence : add_by(30),
    #        Attr.Dodge : mul_by(1.23),
    #        Attr.LifeBonus : add_by(0.1),
    #        Attr.LifePerHit : add_by(5, cap=4),
    #        Attr.LifePerKill : add_by(-10, floor=0),
    #    })

    #    self.assertEqual(a0, mod_attr(a1, m))


if __name__ == '__main__':
    unittest.main()
