// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prepper/Enums/StatusEffect.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PREPPER_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusEffectComponent();
	friend class APlayerCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	EStatusEffect CurrentStatusEffect;

	UPROPERTY()
	class APlayerCharacter* Character;
	

public:
	TMap<EStatusEffect, int32> StateEffectMap;

	void InitStateEffectMap();

	UPROPERTY(EditAnywhere)
	int32 HungryValue = 100;
	UPROPERTY(EditAnywhere)
	int32 ThirstyValue = 100;
	
	UFUNCTION()
	EStatusEffect SetStatusEffect(EStatusEffect NewStatusEffect);

	FTimerHandle StatusTimerHandle;
	void StatusTimerStart();
	void StatusTimerFinish();
		
};
