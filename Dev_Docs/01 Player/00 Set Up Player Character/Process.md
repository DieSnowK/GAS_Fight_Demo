# `Input Binding Process` #Done

1.  `Native Gameplay Tags`
	- `Define input tags`
2.  `Input Config Data Asset`
	- `Map input tag to input action`
3.  `Custom Input Component`
	- `Define input binding methods`
4.  `Binding Inputs`
	- `Define input callbacks`
5.  `Assign Assets In Editor`
	- `Fill in assets needed in editor`

---
# `Weapon Spawn Process` #Done

1. `Ability System Component`
2. `Set Up Spawn Ability`
3. `Create Weapon Class`
4. `Grant Ability`
5. `Finish Spawning`

---
# `Grant Ability Process` #Done

1. `ProcessedBy()` --> `Init Actor Info`
2. `DataAsset_StartUpDataBase` --> `GiveToAbilitySystem(OwningASC)`
	- `DataAsset_PlayerStartUpData`
		- `Override GiveToAbilitySystem()`
	- `DataAsset_EnemyStartUpData`
		- `Override GiveToAbilitySystem()`

---
# `Ability Activation Policy` #Done

1. `Give Ability To Character`
	- `Give to AbilitySystemComponent`
2. `Activate Ability`
	- `On Given`
		- `One time off`
	- `On Triggered`
3. `End Ability`
4. `Clear Ability`

---
