# AbilityForge

Course project for **Unreal Engine 5 GAS: Gameplay Ability System Essentials in C++**.

One character with Health and Stamina, a Dash and a montage-driven melee attack, built with the Gameplay Ability System and tested with two clients.

## Requirements

- Unreal Engine 5.7
- Visual Studio 2022 or Rider, set up for Unreal C++
- [Git LFS](https://git-lfs.com/) (`.uasset` and `.umap` files are stored with LFS)

## Opening the project

1. Clone the repository and run `git lfs pull`.
2. Right-click `AbilityForge.uproject` and choose **Generate Visual Studio project files**.
3. Open `AbilityForge.uproject` and let it build the C++ module.

## What is C++ and what is Blueprint

- **Blueprint (unchanged template):** movement, camera and the template input in `BP_ThirdPersonCharacter`.
- **C++:** everything GAS. That's the PlayerState with the Ability System Component, the Attribute Set, abilities, native tags, the anim notify, the training dummy and the HUD widget base class.
- **Editor assets (data):** Gameplay Effects, the montage, the Gameplay Cue, the HUD widget and the input actions. See [Docs/EditorSetup.md](Docs/EditorSetup.md) for the steps for each section.

## Checkpoints

Each course section ends with a tagged commit. Check one out to start from that point:

```
git checkout section-04
```

| Tag | Section | What it adds |
| --- | --- | --- |
| `section-02` | The Ability System Component | GAS plugin and modules, PlayerState-owned ASC, actor info on server and client |
| `section-03` | Attributes | Health, MaxHealth, Stamina, MaxStamina with replication and clamping |
| `section-04` | Gameplay Abilities | Base ability, startup grants at player level, Enhanced Input activation, Dash with a grounded-only check |
| `section-05` | Gameplay Tags | Native tags; Dash owns `State.Dashing` and is blocked by `State.Stunned` |
| `section-06` | Gameplay Effects | Attribute initialization, SetByCaller damage helper, Dash cost and cooldown |
| `section-07` | Ability Tasks and a melee attack | Melee with PlayMontageAndWait and WaitGameplayEvent, level-scaled damage, training dummy, optional Dash montage |
| `section-08` | Gameplay Cues and UI | Hit cue on the target, health and stamina HUD from attribute delegates, Gameplay Cue path |
| `section-09` | Multiplayer essentials | Final project, tested with two clients |

Assignment solutions (Sprint, stamina regeneration, stun on hit) are on the `assignment-solutions` branch, so they don't spoil the checkpoints.
