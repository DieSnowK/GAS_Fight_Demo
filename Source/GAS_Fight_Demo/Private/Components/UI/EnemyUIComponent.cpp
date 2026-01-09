// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/FightWidgetBase.h"


void UEnemyUIComponent::RegisterEnemyDrawnWidget(UFightWidgetBase* InWidgetToRegister)
{
	EnemyDrawnWidgets.Add(InWidgetToRegister);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UFightWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
