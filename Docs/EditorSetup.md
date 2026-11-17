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
