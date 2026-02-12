# `Binding Ability Input` #Done

1. `Input Component` ↓ `pass input tag to callback`
2. `Player Character` ↓ `pass onto ASC`
3. `Ability System Component` ↓ `use input tag to activate ability`
4. `Ability Triggered`

---
1. `Player Start Up Data`
	- `grant input tag to ability`
2. `Give to Player`
	- `Abilities tagged with input tags`

---
# `New Ability Process` #Done

- `Ability Tags`
- `Gameplay Ability Blurprint`
- `Anim Montage`
- `Ability Input Action`
- `Grant Ability`

---
# `Weapon Equipping` #Done

- `Retrieve the Spawned Weapon`
- `A New Gameplay Ability`
- `Ability Input Action`
- `Armed Locomotion`
- `Unequip`

---
## `Register Weapon` #Done

- `GA_SpawnWeapon(Spawns weapon)` --> `PawnCombatComponent`

---
## `Equip Ability` #Done

- `Link Anim Layer`
- `Grant Weapon Abilities`
- `Add Input Mapping`

---
## `Unequip Ability` #Done

- `Unlink Anim Layer`
- `Remove Granted Weapon Abilities`
- `Remove Input Mapping`

---
# `Jump To Finisher` #Done

- `Light 3 --> Heavy 2`

---
## `Light Attacks` #Done

- `About to reach last move?`
	- `Yes` --> `Tag player Character`
	- `No` --> `Increase Combo Count`

---
## `Heavy Attacks` #Done

- `Does player character have tag?`
	- `Yes` --> `Perform finisher`
	- `No` --> `Use current combo count`