// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"

UStatusEffectComponent::UStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	if(Character && Character->IsLocallyControlled())
	{
		InitStateEffectMap();
	}
	
}

void UStatusEffectComponent::InitStateEffectMap()
{
	StateEffectMap.Emplace(EStatusEffect::ESE_HUNGRY, HungryValue);
	StateEffectMap.Emplace(EStatusEffect::ESE_THIRSTY, ThirstyValue);
}



void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UStatusEffectComponent::StatusTimerStart()
{
	if(Character && Character->IsLocallyControlled())
	{
		GetWorld()->GetTimerManager().SetTimer(
			StatusTimerHandle,
			this,
			&UStatusEffectComponent::StatusTimerFinish,
			5.0f,
			true);
	}
}

void UStatusEffectComponent::StatusTimerFinish()
{
	if(Character)
	{
		StateEffectMap[EStatusEffect::ESE_HUNGRY] -= 1;
		StateEffectMap[EStatusEffect::ESE_THIRSTY] -= 1;
	}
}
