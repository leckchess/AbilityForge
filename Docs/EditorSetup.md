# Editor setup

The C++ in this project is the GAS logic. These are the editor steps that go with it: assets that hold data (effects, montages, cues, widgets, input actions) and the Blueprint defaults that connect them to the code.

Ability Blueprints go in `Content/Abilities`, and the other GAS assets in subfolders of it (`Effects`, `Montages`, `Cues`, `UI`, `Data`). Input actions go in `Content/Input/Actions`.

## Section 2: The Ability System Component

1. Close the editor, build the project from your IDE, then reopen it. The `.uproject` now enables the **GameplayAbilities** plugin.
2. Open `BP_ThirdPersonGameMode`. In **Class Defaults**, set **Player State Class** to `AbilityForgePlayerState`.
3. Press Play, open the console (`` ` ``) and run `showdebug abilitysystem`. The ASC owner should be the PlayerState and the avatar should be the character.

If the Output Log shows *"set the Game Mode's Player State Class to AbilityForgePlayerState"*, step 2 is missing.

## Section 3: Attributes

No editor steps. Play and run `showdebug abilitysystem`. Health, MaxHealth, Stamina and MaxStamina show 100, the defaults from the Attribute Set constructor.

## Section 4: Gameplay Abilities

1. Create the input action `Content/Input/Actions/IA_Dash` (Value Type: Digital (bool)).
2. Open `Content/Input/IMC_Default` and map `IA_Dash` to **Q**.
3. Create a Blueprint class `Content/Abilities/GA_Dash_BP` with parent `GA_Dash`. You'll set its cost and cooldown in Section 6.
4. Open `BP_ThirdPersonCharacter`. In **Class Defaults > Abilities > Startup Abilities**, add an entry with Ability `GA_Dash_BP` and Input Action `IA_Dash`.
5. Optional, for the level-scaling demo later: create `BP_AbilityForgePlayerState` (parent `AbilityForgePlayerState`), set **Character Level**, and use it as the Game Mode's Player State Class.

Test: press **Q** on the ground to dash. Jump and press **Q**: nothing happens, because `CanActivateAbility` refuses while falling.

## Section 5: Gameplay Tags

No editor steps. Open **Project Settings > Gameplay Tags** to see the native tags declared in `AbilityForgeGameplayTags.cpp`. While dashing, `showdebug abilitysystem` lists `State.Dashing` on the owner.

## Section 6: Gameplay Effects

Create these Gameplay Effect Blueprints in `Content/Abilities/Effects`:

| Asset | Duration Policy | Setup |
| --- | --- | --- |
| `GE_InitAttributes` | Instant | Four modifiers, **Override**: MaxHealth 100, Health 100, MaxStamina 100, Stamina 100. Put the Max attributes first. |
| `GE_Damage` | Instant | One modifier: Health, **Add**, magnitude type **Set By Caller**, tag `Data.Damage` |
| `GE_DashCost` | Instant | One modifier: Stamina, **Add**, Scalable Float `-20` |
| `GE_DashCooldown` | Has Duration, 1.5 s | Add the component **Grant Tags to Target Actor** with `Cooldown.Dash` |

Then:

1. `GA_Dash_BP`: set **Costs > Cost Gameplay Effect Class** to `GE_DashCost` and **Cooldowns > Cooldown Gameplay Effect Class** to `GE_DashCooldown`.
2. `BP_ThirdPersonCharacter`: set **Default Attributes Effect** to `GE_InitAttributes`.

Test: each dash costs 20 stamina, and you can't dash again for 1.5 seconds or with less than 20 stamina.

**Assignment 2 (stamina regeneration):** create `GE_StaminaRegen` with **Infinite** duration, **Period** 0.5 s, and one modifier Stamina **Add** `5`. Add it to the character's **Startup Effects**.
