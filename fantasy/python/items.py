from d3core import *


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
