from d3core import *

Fulminator = Item(
    name='Fulminator',
    stats=AttrMod({
        Attr.WeaponDamage : const(40),
        Attr.Strength : add_by(100),
    }),
    iconfig=WConfig(wtype=WType.Sword, handle=WHandle.OneHanded),
)

LeoricsCrown = Item(
    name="Leoric's Crown",
    stats=AttrMod({
        Attr.Armor : add_by(230),
        Attr.Strength : add_by(20),
        Attr.Vitality : add_by(15),
    }),
    iconfig=AType.Helm,
)
