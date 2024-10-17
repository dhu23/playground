package fantasy.impl.priest;

import fantasy.impl.AbstractCharacter;
import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.item.Weapon;
import fantasy.impl.resource.ManaResource;
import fantasy.impl.resource.cost.BaseManaCost;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Skill;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Priest extends AbstractCharacter {
    private static final Logger logger = LoggerFactory.getLogger(Priest.class);

    private ManaResource resource_;

    public Priest(String name, int level) {
        super(name, level, 65);

        this.resource_ = new ManaResource(190, 370);
    }

    @Override
    protected void onAttackWithMainHand_() {
        // nothing
    }

    @Override
    protected void onAttackWithOffHand_() {
        // nothing
    }

    @Override
    public boolean hasResourceFor(Skill skill) {
        return this.resource_.hasResourceFor(skill, this);
    }

    @Override
    public boolean consumeResource(Skill skill) {
        logger.info(String.format("%s has %s mana", name(), resource_.summary()));
        boolean ok = resource_.consume(skill, this);
        if (ok) {
            logger.info(String.format("%s has %s", name(), resource_.summary()));
        }
        return ok;
    }

    @Override
    public IntegerInterval emptyHandedDamage() {
        return ImmutableIntegerInterval.of(1, 3);
    }

    @Override
    public IntegerInterval weaponDamage() {
        // TODO stats enhancement needs to be added
        return this.mainHand.map(Weapon::weaponDamage).orElse(emptyHandedDamage());
    }

    @Override
    public int armor() {
        return 1000;
    }

    @Override
    public double damageMitigation() {
        return 0;
    }

    @Override
    public double criticalChance() {
        return 0.01;
    }

    @Override
    public double spellCriticalChance() {
        return 0.05;
    }

    @Override
    public int dealWeaponDamage() {
        return weaponDamage().sample(WorldSpaceTime.getInstance().getRandomGenerator());
    }

    @Override
    public boolean equipWeapon(Weapon item) {
        // TODO equip no weapon for now
        return false;
    }

    @Override
    public String shortDescription() {
        return description();
    }

    @Override
    protected String characterClassName() {
        return "Priest";
    }
}
