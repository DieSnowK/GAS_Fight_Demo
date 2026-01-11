// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "FightEnumTypes.h"


/**
 * @brief 倒计时延迟动作类，用于在蓝图中实现倒计时功能
 */
class FFightCountDownAction : public FPendingLatentAction
{
public:
	FFightCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime,
		EFightCountDownActionOutput& CountDownOutput, const FLatentActionInfo& LatentInfo);

	/**
	 * @brief: 每帧更新倒计时状态，类似Tick()
	 * 
	 * 这是Latent Action系统的核心，每帧都会被LatentActionManager调用
	 * 
	 * @param: Response 用于控制Action状态和触发蓝图回调的响应对象
	 */
	virtual void UpdateOperation(FLatentResponse& Response) override;

	void CancelAction();

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EFightCountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};