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
