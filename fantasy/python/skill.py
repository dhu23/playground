from d3core import Skill

class Skill(Enum):
    Bash = auto()


class Bash(Skill):

    class Rune(Enum):
        Frostbite = auto()
        Onslaught = auto()
        Punish = auto()
        Instigation = auto()
        Pulverize = auto()

    def __init__(self):
        super(Bash, self).__init__(set(Bash.Rune))

    @property
    def name(self):
        return Skill.Bash

    def use(self, character, target):
        outcome = []
    
        if Bash.Rune.Instigation in self.active:
            rage_gen = 6
        else:
            rage_gen = 9

        outcome.append(ResourceGen(rtype=Resource.Rage, val=rage_gen))


        if Bash.Rune.Frostbite in self.active:
            outcome.append(Frozen(duration=1.5, stacklimit=1))
            _school = Schoo.Cold

        elif Bash.Rune.Onslaught in self.active:
            outcome.append(CritChanceVunerable(duration=3, stacklimit=1, bonus=0.1))
            _school = School.Lightning

        elif Bash.Rune.Punish in self.active:
            # Damage 4% for 5 second
            outcome.append(DamageBonus(duration=5, stacklimit=3, bonus=0.04))
            _school = School.Physical

        elif Bash.Rune.Instigation in self.active:
            _school = School.Fire

        elif Base.Rune.Pulverize in self.active:
            _school = School.Fire

        else:
            # a shockwave that deals 100% weapon damage as Fire 
            # to enemies in a 26 yard line behind the primary enemy.
            _school = School.Physical

        _dmg = int(character.actual_damage * 3.2) # 320% damage
        outcome.append(Dmg(amount=_dmg, school=_school))
