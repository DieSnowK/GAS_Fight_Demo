#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" // ATTRIBUTE_ACCESSORS宏定义在这里
#include "BasicAttributeSet.generated.h"


struct FGameplayAttributeData;
class IPawnUIInterface;


// 自动生成属性访问器宏 --> GAS封装好的基础功能,  用于属性的读取、设置和初始化，减少重复代码。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class GAS_FIGHT_DEMO_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief UBasicAttributeSet构造函数
	 *
	 * 初始化角色的各种属性值，包括生命值、怒气值、攻击力、防御力等
	 * 这些属性构成了角色在游戏中的基础数值系统
	 */
	UBasicAttributeSet();

	/**
	 * @brief 游戏效果执行后的回调函数
	 *
	 * 当GameplayEffect修改了属性值后，该函数会被自动调用
	 * 用于处理属性值变更后的逻辑，如数值限制、副作用处理等
	 *
	 * @param Data 包含GameplayEffect执行相关信息的回调数据结构
	 *
	 * @details
	 * 1. 检查被修改的属性类型
	 * 2. 对生命值属性进行范围限制（0到最大生命值）
	 * 3. 对怒气值属性进行范围限制（0到最大怒气值）
	 * 4. 处理伤害承受逻辑，计算实际生命值变化
	 * 5. 输出调试信息
	 * 6. 检查角色是否死亡并处理相关逻辑
	 *
	 * @note 该函数在GameplayEffect执行后自动调用，用于处理属性变更后的逻辑
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, CurrentRage);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxRage);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, DefensePower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, DamageTaken);

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};
