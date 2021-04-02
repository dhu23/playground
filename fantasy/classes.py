from d3core import *

class RClass(Enum):
    Barbarian = auto()
    Crusader = auto()
    Wizard = auto()
    Monk = auto()
    WitchDoctor = auto()
    Necromancer = auto()
    DemonHunter = auto()


class Barbarian(Character):

    INIT_STATS = BasicAttrs({
        Attr.Strength : 35,
        Attr.Dexterity : 12,
        Attr.Intelligence : 10,
        Attr.Vitality : 25,
    })

    UPTO60 = MainAttrGain(s=4, d=1, i=1, v=5)
    UPTO65 = MainAttrGain(s=8, d=2, i=1, v=7)
    UPTO70 = MainAttrGain(s=10, d=2, i=1, v=9)

    RESOURCES = {
        Resource.Rage : 100,
    }

    def __init__(self, name, level=1):
        super(Barbarian, self).__init__(
            level,
            Barbarian.INIT_STATS, 
            Barbarian.UPTO60,
            Barbarian.UPTO65,
            Barbarian.UPTO70,
            Barbarian.RESOURCES,
        )
        self.name = name

    def role(self):
        return RClass.Barbarian

    def resource_types(self):
        return [Resource.Rage]

    @property
    def enhanced_damage(self):
        return four_decimal_float(self.strength/100)


#class DemonHunter(Character):
#
#    INIT_STATS = BasicAttrs({
#        Attr.Strength : 20, 
#        Attr.Dexterity: 30,
#        Attr.Intelligence: 15,
#        Attr.Vitality : 20
#    })
#
#    UPTO60 = MainAttrGain(s=2, d=4, i=1, v=3, dmg=3)
#    UPTO65 = MainAttrGain(s=3, d=8, i=2, v=5, dmg=6)
#    UPTO70 = MainAttrGain(s=4, d=10, i=2, v=7, dmg=8)
#
#    def __init__(self, level=1):
#        super(DemonHunter, self).__init__(
#
#            DemonHunter.INIT_STATS,
#            DemonHunter.UPTO60,
#            DemonHunter.UPTO65,
#            DemonHunter.UPTO70,
#            level,
#        )
#
#    @property
#    def role(self):
#        return RClass.DemonHunter
#
#
#class Wizard(Character):
#
#    INIT_STATS = BasicAttrs({
#        Attr.Strength : 10, 
#        Attr.Dexterity : 10,
#        Attr.Intelligence : 40,
#        Attr.Vitality : 15
#    })
#
#    UPTO60 = MainAttrGain(s=1, d=2, i=4, v=2, dmg=2)
#    UPTO65 = MainAttrGain(s=2, d=2, i=7, v=3, dmg=3)
#    UPTO70 = MainAttrGain(s=2, d=3, i=12, v=5, dmg=4)
#
#    def __init__(self, level=1):
#        super(Wizard, self).__init__(
#            Wizard.INIT_STATS,
#            Wizard.UPTO60,
#            Wizard.UPTO65,
#            Wizard.UPTO70,
#            level,
#        )
#
#    @property
#    def role(self):
#        return RClass.Wizard
