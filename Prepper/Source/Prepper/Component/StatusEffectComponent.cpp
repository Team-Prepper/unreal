// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "Net/UnrealNetwork.h"
#include "Prepper/Character/PlayerCharacter.h"

UStatusEffectComponent::UStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	if(Character)
	{
		UE_LOG(LogTemp,Warning,TEXT("StatusEffectReady"));
		StatusFlags.ClearAllEffects();
		InitStateEffectMap();
		StatusTimerStart();
	}
	
}

void UStatusEffectComponent::InitStateEffectMap()
{
	StateEffectMap.Emplace(EStatusEffect::ESE_HUNGRY, 100);
	StateEffectMap.Emplace(EStatusEffect::ESE_THIRSTY, 100);
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
			1.0f,
			true);
	}
}

void UStatusEffectComponent::StatusTimerFinish()
{
	if(Character)
	{
		StateEffectMap[EStatusEffect::ESE_HUNGRY] -= 1.f;
		StateEffectMap[EStatusEffect::ESE_THIRSTY] -= 1.f;
		//UE_LOG(LogTemp,Warning,TEXT("[StatusEffect] Hungry : %f"),StateEffectMap[EStatusEffect::ESE_HUNGRY]);
		//UE_LOG(LogTemp,Warning,TEXT("[StatusEffect] Thirsty : %f"),StateEffectMap[EStatusEffect::ESE_THIRSTY]);
		UpdateStatusEffect();
	}
}

void UStatusEffectComponent::UpdateStatusEffect()
{
	for (const auto& EffectThreshold : EffectThresholds)
	{
		const bool HasEffect = StatusFlags.HasEffect(EffectThreshold.Effect);
		const float EffectValue = StateEffectMap[EffectThreshold.Effect];

		if (!HasEffect && EffectValue <= EffectThreshold.Threshold)
		{
			StatusFlags.AddEffect(EffectThreshold.Effect);
		}
		else if (HasEffect && EffectValue > EffectThreshold.Threshold)
		{
			StatusFlags.RemoveEffect(EffectThreshold.Effect);
		}
	}

	for (const auto& EffectThreshold : EffectThresholds)
	{
		if (StatusFlags.HasEffect(EffectThreshold.Effect))
		{
			//UE_LOG(LogTemp, Log, TEXT("Current Status Effect: %s"), *EffectThreshold.EffectName);
		}
	}
}
