# `Pawn Extension Component` #Done

- `PawnExtensionComponentBase` --> `Provides helper functions`
	- `PawnCombatComponent` --> `Contains stuff shared by both enemy and player`
		- `PlayerCombatComponent`
			- `Logic specific to player`
		- `EnemyCombatComponent`
			- `Logic specific to enemy`

---
# `Gameplay Ability Structure` #Done

- `FightGameplayAbility` --> `Provides helper function`
	- `FightPlayerGameplayAbility` --> `Logic specific to player`
	- `FightEnemyGameplayAbility` --> `Logic specific to enemy`

---
# `Weapon Structure` #Done

- `FightWeaponBase` --> `Handle damage detection`
	- `FightPlayerWeapon` --> `Weapon data unique to player`

---
# `Weapon Data Structure` #Done

- `PlayerWeaponData` ↓ `Combination`
	- `AnimLayerToLink`
	- `Default Weapon Ability` | `eg.`
		- `Unequip axe`
		- `Axe light attack`
		- `Axe heavy attack`
	- `Input Mapping Context` | `sth to do`
		- `Override key binding`
		- `Weapon key binding`