from d3core import *

test_two_handed_sword0 = Item(
    name='Scottish Claymore',
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


############################ Unique items ###########################
Fulminator = Item(
    name='Fulminator',
    stats=[
        AttrAdd(attr=Attr.WeaponDamage, val=40),
        AttrAdd(attr=Attr.Strength, val=100),
    ],
    iconfig=WConfig(wtype=WType.Sword, handle=WHandle.OneHanded),
)

LeoricsCrown = Item(
    name="Leoric's Crown",
    stats=[
        AttrAdd(attr=Attr.Armor, val=230),
        AttrAdd(attr=Attr.Strength, val=20),
        AttrAdd(attr=Attr.Vitality, val=15),
    ],
    iconfig=AType.Helm,
)
