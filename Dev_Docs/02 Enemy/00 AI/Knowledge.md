# `Behavior Tree`

- `Blackboard Asset`
	- `The brain for a Behavior Tree`
	- `Holds info for BT to make decisions from`
- `Behavior Tree`
	- `Make decisions for AI`
	- `Runs from left-to-right, top-to-bottom`

---
## `Behavior Tree Node Types`

- `Composite Nodes`
	- `Selector`
		- `The root of a branch`
		- `Evaluate all children from left to right`
		- `Execute the branch that succeeds`
	- `Sequence`
		- `Defines the base rules for how the branch is executed`
		- `Try executing all children from left to right`
		- `Stop executing when one of the children fails`
- `Decorator Nodes`
	- `Can be attached to a composite/task node`
	- `Defines if a branch/node can be executed`
- `Service Nodes`
	- `Can be attached to a composite/task node`
	- `Execute at defined frequency`
	- `Often used for checks and updating blackboard value`
- `Task Nodes`
	- `The leaf nodes under composite nodes`
	- `Do things`
	- `Can have Services and Decorators attached to it`

---
## `Notify Observer`

- `On Result Change`
	- `Condition check result changes`
		- `true -> false`
		- `false -> true`
	- `On Value Change`
		- `Observed blackboard key value changes`

---
## `Observer Aborts`

- `None`
	- `Abort nothing`
- `Self`
	- `Abort self when the result becomes false`
- `Lower Priority`
	- `Abort lower priority branch when the result becomes true`
- `Both`
	- `Do both above`

---
# `AI Avoidance`

- `RVO Avoidance`
- `Detour Crowd Avoidance`

---
## `RVO Avoidance`

- `Pushes agents away from each other`
- `Ignores obstacles and nav mesh`

---
## `Detour Crowd Avoidance`

- `Aware of other agents`
- `Alters velocity for new path`
- `Respects nav mesh`

---
# `EQS`

## `Environment Query System`

- `Collect data from the environment`
- `Instruct AI characters to find the best possible location`

## `EQS Generator`

- `Produce points (refered as Items)`
- `Items will be tested and weighted`
- `Return highest weight item to behavior tree`

---
## `EQS Terms`

- `Generator`
- `Context`
- `EQS Test Pawn`
- `Test`
- `Filter / Score`

---

# `Motion Warping`

- `Dynamically adjust a character's root motion to align to targets`

---