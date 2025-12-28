#pragma once

#define DEBUG 0

namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::Red, int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg); // 输出到屏幕

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg); // 输出到LOG, *MSG 将FString转换为const TCHAR* (重载*实现)
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}

//class GASDebugHelper
//{
//public:
//
//};
