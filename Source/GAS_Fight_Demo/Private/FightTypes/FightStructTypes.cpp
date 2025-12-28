// Fill out your copyright notice in the Description page of Project Settings.


#include "FightTypes/FightStructTypes.h"

bool FFightPlayerAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}