package fantasy.impl.priest.playcontrol;

import fantasy.impl.priest.GreaterHeal;
import fantasy.impl.priest.Priest;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.PlayControl;
import fantasy.intf.Skill;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class GreaterHealSpam implements PlayControl {
    private static final Logger logger = LoggerFactory.getLogger(GreaterHealSpam.class);
    protected final Priest priest_;

    public GreaterHealSpam(Priest priest) {
        priest_ = priest;
        priest_.setControl(this);
    }

    @Override
    public void onSelect() {
        castGreaterHeal();
    }

    @Override
    public void onUnselect() {

    }

    @Override
    public void onGlobalCoolDownFinish() {
        castGreaterHeal();
    }

    @Override
    public void onSkillCoolDownFinish(Skill skill) {
        castGreaterHeal();
    }

    @Override
    public void onEffectExpiration(Character target, Effect effect) {

    }

    protected void castGreaterHeal() {
        logger.info("casting greater heal...maybe");
        if (priest_.isCasting().isEmpty()) {
            logger.info("casting greater heal...yes");
            priest_.cast(GreaterHeal.GREATER_HEAL);
        }
    }
}
