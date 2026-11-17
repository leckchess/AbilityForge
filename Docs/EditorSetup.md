# Editor setup

The C++ in this project is the GAS logic. These are the editor steps that go with it: assets that hold data (effects, montages, cues, widgets, input actions) and the Blueprint defaults that connect them to the code.

Put new assets under `Content/GAS/` (`Effects`, `Abilities`, `Cues`, `UI`, `Data`, `Input`).

## Section 2: The Ability System Component

1. Close the editor, build the project from your IDE, then reopen it. The `.uproject` now enables the **GameplayAbilities** plugin.
2. Open `BP_ThirdPersonGameMode`. In **Class Defaults**, set **Player State Class** to `AbilityForgePlayerState`.
3. Press Play, open the console (`` ` ``) and run `showdebug abilitysystem`. The ASC owner should be the PlayerState and the avatar should be the character.

If the Output Log shows *"set the Game Mode's Player State Class to AbilityForgePlayerState"*, step 2 is missing.

## Section 3: Attributes

No editor steps. Play and run `showdebug abilitysystem`. Health, MaxHealth, Stamina and MaxStamina show 100, the defaults from the Attribute Set constructor.

## Section 4: Gameplay Abilities

1. Create the input action `Content/GAS/Input/IA_Dash` (Value Type: Digital (bool)).
2. Open `Content/Input/IMC_Default` and map `IA_Dash` to **Q**.
3. Create a Blueprint class `Content/GAS/Abilities/BP_GA_Dash` with parent `GA_Dash`. You'll set its cost and cooldown in Section 6.
4. Open `BP_ThirdPersonCharacter`. In **Class Defaults > Abilities > Startup Abilities**, add an entry with Ability `BP_GA_Dash` and Input Action `IA_Dash`.
5. Optional, for the level-scaling demo later: create `BP_AbilityForgePlayerState` (parent `AbilityForgePlayerState`), set **Character Level**, and use it as the Game Mode's Player State Class.

Test: press **Q** on the ground to dash. Jump and press **Q**: nothing happens, because `CanActivateAbility` refuses while falling.

## Section 5: Gameplay Tags

No editor steps. Open **Project Settings > Gameplay Tags** to see the native tags declared in `AbilityForgeGameplayTags.cpp`. While dashing, `showdebug abilitysystem` lists `State.Dashing` on the owner.

## Section 6: Gameplay Effects

Create these Gameplay Effect Blueprints in `Content/GAS/Effects`:

| Asset | Duration Policy | Setup |
| --- | --- | --- |
| `GE_InitAttributes` | Instant | Four modifiers, **Override**: MaxHealth 100, Health 100, MaxStamina 100, Stamina 100. Put the Max attributes first. |
| `GE_Damage` | Instant | One modifier: Health, **Add**, magnitude type **Set By Caller**, tag `Data.Damage` |
| `GE_DashCost` | Instant | One modifier: Stamina, **Add**, Scalable Float `-20` |
| `GE_DashCooldown` | Has Duration, 1.5 s | Add the component **Grant Tags to Target Actor** with `Cooldown.Dash` |

Then:

1. `BP_GA_Dash`: set **Costs > Cost Gameplay Effect Class** to `GE_DashCost` and **Cooldowns > Cooldown Gameplay Effect Class** to `GE_DashCooldown`.
2. `BP_ThirdPersonCharacter`: set **Default Attributes Effect** to `GE_InitAttributes`.

Test: each dash costs 20 stamina, and you can't dash again for 1.5 seconds or with less than 20 stamina.

**Assignment 2 (stamina regeneration):** create `GE_StaminaRegen` with **Infinite** duration, **Period** 0.5 s, and one modifier Stamina **Add** `5`. Add it to the character's **Startup Effects**.

## Section 7: Ability Tasks and a melee attack

1. **Damage curve.** Import `Docs/Data/CT_MeleeDamage.csv` into `Content/GAS/Data` as a **Curve Table** (Linear interpolation). Row `MeleeDamage` goes from 10 at level 1 to 30 at level 5.
2. **Montage.** Right-click `Characters/Mannequins/Anims/Unarmed/Attack/MM_Attack_01` and choose **Create > Create AnimMontage**, then move it to `Content/GAS/Abilities/AM_Melee`. Keep the slot `DefaultSlot`.
3. **Hit timing.** In `AM_Melee`, add a notify of type **Send Gameplay Event** at the impact frame and set its Event Tag to `Event.Melee.Hit`.
4. **Anim Blueprint.** Open `ABP_Unarmed`. If its AnimGraph has no **Slot 'DefaultSlot'** node before the Output Pose, add one, or montages won't play.
5. **Input.** Create `IA_Attack` (Digital (bool)) and map it to **Left Mouse Button** in `IMC_Default`.
6. **Ability.** Create `BP_GA_Melee` (parent `GA_Melee`):
   - Attack Montage: `AM_Melee`
   - Damage: Value `1`, Curve `CT_MeleeDamage` / `MeleeDamage`
   - Damage Effect Class: `GE_Damage`
   - Turn on **Draw Debug Trace** while recording the trace lecture.
7. **Grant it.** Add a Startup Abilities entry to `BP_ThirdPersonCharacter`: `BP_GA_Melee` with `IA_Attack`.
8. **Target.** Create `BP_TrainingDummy` (parent `AbilityForgeTrainingDummy`). Set its Mesh to `SKM_Manny_Simple` and its Anim Class to `ABP_Unarmed`, matching the template character's mesh offset (Z -90, Yaw -90). Place one or two in the level.

Test: attack the dummy. Its health text drops by the curve value for your level. It resets after 2 seconds at 0.

**Assignment 3 (stun):** create `GE_Stun` with **Has Duration** 1.5 s and **Grant Tags to Target Actor** `State.Stunned`. While stunned, Dash and Melee can't activate. The solution branch adds an **On Hit Effect Class** to `GA_Melee` for it.

## Section 8: Gameplay Cues and UI

1. **Hit cue.** Create a Blueprint class with parent `GameplayCueNotify_Static`, named `GC_Hit`, in `Content/GAS/Cues`. Set **Gameplay Cue Tag** to `GameplayCue.Hit`. Override **On Execute**: spawn an effect or play a sound at **Parameters > Location** (`NS_JumpPad` from LevelPrototyping works as a placeholder). Return true.
2. **HUD.** Create a Widget Blueprint `Content/GAS/UI/WBP_HUD` and set its parent class (**Graph > Class Settings**) to `AbilityForgeHUDWidget`. Add two **Progress Bars** named exactly `HealthBar` and `StaminaBar`.
3. `BP_ThirdPersonCharacter`: set **HUD Widget Class** to `WBP_HUD`.

Test: the cue plays on every hit, and the stamina bar drops on each dash.
