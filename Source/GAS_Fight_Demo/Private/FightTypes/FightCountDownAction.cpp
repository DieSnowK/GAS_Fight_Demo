// Fill out your copyright notice in the Description page of Project Settings.


#include "FightTypes/FightCountDownAction.h"

FFightCountDownAction::FFightCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, 
	EFightCountDownActionOutput& CountDownOutput, const FLatentActionInfo& LatentInfo)
	: bNeedToCancel(false)
	, TotalCountDownTime(InTotalCountDownTime)
	, UpdateInterval(InUpdateInterval)
	, OutRemainingTime(InOutRemainingTime)
	, CountDownOutput(CountDownOutput)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
	, ElapsedInterval(0.f)
	, ElapsedTimeSinceStart(0.f)
{}

void FFightCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	// 当外部调用CancelAction()时，bNeedToCancel会被设置为true
	if (bNeedToCancel)
	{
		// 设置输出状态为"已取消"
		CountDownOutput = EFightCountDownActionOutput::Cancelled;

		// 完成行动并触发蓝图回调
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		// 设置输出状态为"已完成"
		CountDownOutput = EFightCountDownActionOutput::Completed;

		// 完成行动并触发蓝图回调
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	// 处理更新间隔逻辑 --> 检查当前间隔累计是否已达到设定的更新间隔
	if (ElapsedInterval < UpdateInterval)
	{
		// 如果还没到更新间隔，累积经过的时间 --> Response.ElapsedTime(): 获取自上一帧以来的时间增量
		ElapsedInterval += Response.ElapsedTime();
	}
	else // 如果已达到或超过更新间隔，执行更新逻辑
	{
		// 更新已过总时间 --> 如果UpdateInterval > 0，使用固定间隔，否则使用实际经过的时间
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		// 计算并更新剩余时间
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		// 设置输出状态为"已更新"
		CountDownOutput = EFightCountDownActionOutput::Updated;

		// 触发蓝图的更新链接（但不结束行动）--> 这意味着倒计时还会继续，直到完成或被取消
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		// 重置间隔计时器
		ElapsedInterval = 0.f;
	}
}

void FFightCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
