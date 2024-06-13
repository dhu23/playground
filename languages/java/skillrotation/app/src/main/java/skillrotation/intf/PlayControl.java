package skillrotation.intf;

public interface PlayControl {
    void onSelect();
    void onUnselect();
    void onGlobalCoolDownFinish();
    // TODO put this somewhere else
    void onRuneCoolDownFinish();
}
