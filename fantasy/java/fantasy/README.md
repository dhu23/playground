# TODO
* make WorldSpaceTime non-Singleton 
* add new skills for the full rotation (in progress)
* machine time simulation
* add analytics tools, both in process and out process (out process done)
* add critical attack and miss/parry/dodge

# Design Ideas
## Skill types
* targeted instant skills: 
  * without projectile: Icy Touch, Fire Blast, Frost Strike
  * with projectile: Arcane Barrage
* targeted spells with a cast time:
  * without projectile: Holy Light, Scorch, Holy Fire, Penance
  * with projectile: Fireball, Aimed Shot
* targeted channeling spells:
  * without projectile: Mind Flay
  * with projectile: Penance
* Environmental: 
  * channelled: Blizzard, Volley
  * non-channelled: Arcane Explosion, Consecration

| skill type    | example          | relationship to time | relationship to space |
|---------------|------------------|:--------------------:|:---------------------:|
| targeted      | Icy Touch        |       Instant        |         Point         |
| targeted      | Arcane Barrage   |       Instant        | Projectile Collision  |
| targeted      | Holy Light       |         cast         |         Point         |
| targeted      | Fireball         |         cast         | Projectile Collision  |
| targeted      | Mind Flay        |      channeling      |         Point         |
| targeted      | Penance          |      channeling      | Projectile Collision  |
| environmental | Blizzard         |      channeling      |    Location Check     |
| environmental | Arcane Explosion |       Instant        |         Point         |
| environmental | Consecration     |       Instant        |    Location Check     |

Implementation consideration:
* relationship to time: 
  * Instant: happens within the thread
  * cast or channeling: puts the caster in a casting state, sets up one or more timers for releases
* relationship to space:
  * Point: happens within the thread
  * Projectile Collision: creates a projectile which generates an event on collision
  * Location Check: check location within the thread

# DONE
* add resource cooldown (done)
* add file logging (done)
* add auto attack (done)
* support stackable effects (done)
* move amount over time stack into the receiver instead of the message itself (done)
* add journal collection for analysis (done)
* add talents - killing machine, chill of the grave, annihilation, runic power mastery, glacier rot