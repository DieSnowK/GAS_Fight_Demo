# `Deal Damage` #Done

- `Enemy Character`
- `Attributes`
- `Hit Detection`
- `Custom Calculation`
- `UI` --> `UIComponent/Delegates + FightWidgetBase`

---
## `Hit Detection` #Done

- `Toggle Weapon Collision`
- `Hit Check` --> `Collision Box`
- `Process Hit`
- `Notify Result`

---
### `Notify Result` #Done

- `GA_LightAttack` | `Play Montage`
	- `Wait Gameplay Event` | `Event Received`
	- `Apply Damage`
- `Combat Component` | `Hit Detected`
	- `Send Gameplay Event`

---
## `Hit Process` #Done

- `WeaponBase` | `BeginOverlap()` --> `Is Target Hostile?`
	- `Yes` --> `Notify Combat Component | Process Hit`
	- `No` --> `Do Nothing`

---
# `Player Combat` #Done

- `Deal Damage`
- `Combat Feedback`

---
## `Combat Feedback` #Done

- `Hit React`
- `Hit / Sound FX`
- `Camera Shake`
- `Death`

---
## `Hit FX` #Done

- `Material Hit FX`
- `Hit Pause`

---
## `BP Death Interface` #Done

- `Notify Death`
	- `Pause Anim`
	- `Disable Collision`
	- `Material FX`
	- `Particle FX`
	- `Destroy Actor`

---
# `Apply Damage` #Done

- `GA_Light/HeavyAttack` ↓
- `Make Gameplay Effect Spec Handle (Attack type/Combo Count/Base Damage)` ↓
- `Apply Handle To Target` ↓
- `Gameplay Effect Execution Calculation (Capture attributes for calculation)` ↓
- `Attribute Set (Clamp values)` ↓
- `Notify UI`

---
## `Broadcast Values` #Done

- `Attribute Change` | `Player / Enemy`
	- `through interface` ↓
	- `Pawn UI Component` ↓
	- `Broadcast`
- `Widgets` | `Health bar, Rage bar, Icons...` ↓
	- `Listen and Update`

---
## `Widgets` #Done

- `Attribute Change` | `Player / Enemy` ↓
	- `PawnUIComponent` --> `Widgets` 
- `Update Icons` | `Player` ↑

---
