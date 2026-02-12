---


---
# `Animation Layer Interface` #Done

- `AnimLayerInterface` --> `declares states` | `eg.`
	- `Fire State`
	- `Idle State`
	- `...`
- `ABP_Main ---implements---> AnimLayerInterface`
	- `plug empty states into outputs`
- `AnimLayerInterface`
	- `ABP_AnimLayerMaster()` --> `Implement interface, define states, create variables` ↓ `Fill in actual anim assets`
		- `ABP_UnarmedLayer`
		- `ABP_PistolLayer`

---
# `Combo System` #Done

- `Moves can be easily added/removed` --> `DataAsset`
- `Animation Independent`
- `Light / Heavy Attacks`
- `Communication`
- `Damage scaled by combo count`

---
