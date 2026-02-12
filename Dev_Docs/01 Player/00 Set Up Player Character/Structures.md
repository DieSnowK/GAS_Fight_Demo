# `Basic Class Structure` #Done

- `FightBaseGameMode`
	- `FightSurvivalGameMode`
- `FightBaseCharacter`
	- `FightPlayerCharacter`
	- `FightEnemyCharacter`

---
# `AnimInstance Structure` #Done

- `FightBaseAnimInstance` --> `Provice helper functions`
	- `FightCharacterAnimInstance` --> `Shared by player and enemy`
		- `FightPlayerAnimInstance`
			- `Contains code specific for player`
		- `FightPlayerLinkedAnimLayer`
			- `For Player Weapons`

---
# `Weapon Class Structure` #Done

- `FightWeaponBase` --> `Handle damage detection`
	- `FightPlayerWeapon` --> `Weapon data unique to player`

---
# `Start Up Data Structure` #Done

- `DataAsset_StartUpDataBase` --> `Data shared by player and enemy`
	- `DataAsset_PlayerStartUpData`
		- `Data specific to player`
		- `Override functions from base class`
	- `DataAsset_EnemyStartUpData`
		- `Data specific to enemy`
		- `Override functions from base class`

---
